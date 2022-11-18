import os
import sys
import time
from datetime import datetime
import http.client
import json
import logging
import socket

from gpiozero import LED, PWMLED
from gpiozero.pins.pigpio import PiGPIOFactory

import config

if config.debug:
    logging.basicConfig(filename=config.logFile, level=logging.DEBUG)
else:
    logging.basicConfig(filename=config.logFile, level=logging.INFO)

class HTTPClient:
    def __init__(self, server, use_https=True):
        self.server = server
        self.use_https = use_https

    def request(self, method, url, body=None, headers={}):
        if self.use_https:
            connection = http.client.HTTPSConnection(self.server)
        else:
            connection = http.client.HTTPConnection(self.server)
        connection.request(method, url, body=body, headers=headers)
        response = connection.getresponse()
        res = response.__dict__.copy()
        res['headers'] = response.headers.__dict__.copy()
        res['body'] = response.read().decode(config.encoding)
        return res

    def get(self, url="/", body=None, headers={}):
        return self.request("GET", url, body=body, headers=headers)

    def post(self, url="/", body=None, headers={}):
        return self.request("POST", url, body=body, headers=headers)

    def put(self, url="/", body=None, headers={}):
        return self.request("PUT", url, body=body, headers=headers)
        
    def delete(self, url="/", body=None, headers={}):
        return self.request("DELETE", url, body=body, headers=headers)


class oneM2MClient:
    def __init__(self, cseBaseName, originator="", requestIdentifier="", releaseVersionIndicator="", use_https=True):
        self.cseBaseName = cseBaseName
        self.originator = originator
        self.requestIdentifier = requestIdentifier
        self.releaseVersionIndicator = releaseVersionIndicator
        self.client = HTTPClient(cseBaseName, use_https=use_https)
        self.headers = {
            "X-M2M-Origin": self.originator,
            "X-M2M-RI": self.requestIdentifier,
            "X-M2M-RVI": self.releaseVersionIndicator,
            "Content-Type": "application/json",
            "Accept": "application/json"
        }

    def retrieve(self, url):
        response = self.client.get(url, headers=self.headers)
        response['body'] = json.loads(response['body'])
        return response

    def delete(self, url):
        response = self.client.delete(url, headers=self.headers)
        response['body'] = json.loads(response['body'])
        return response
    
    def update(self, url, body):
        response = self.client.put(url, headers=self.headers, body=body)
        response['body'] = json.loads(response['body'])
        return response
    
    def create(self, url, body):
        response = self.client.post(url, headers=self.headers, body=body)
        response['body'] = json.loads(response['body'])
        return response
    
    def create_application_entity(self, url, resourceName="", appID="", requestReachability=False, supportedReleaseVersions=["3"]):
        body ={
            "m2m:ae": {
                "api": appID,
                "src": supportedReleaseVersions,
                "rn": resourceName,
                "rr": requestReachability
            }
        }
        self.headers['Content-Type'] = "application/json;ty=2"
        return self.client.post(url, headers=self.headers, body=json.dumps(body))
    
    def create_container(self, url, resourceName=""):
        body = {
            "m2m:cnt": {
                "mia": 3600,        # max instance age in seconds
                "mbs": 10000,       # max byte size
                "mni": 30,          # max nr of instance
                "rn": resourceName
            }
        }
        self.headers['Content-Type'] = "application/json;ty=3"
        return self.client.post(url, headers=self.headers, body=json.dumps(body))
 
    def create_container_instance(self, url, content="", resourceName=None):
        if resourceName:
            body = {
                "m2m:cin": {
                    "cnf": "text/plain:0",
                    "con": content,
                    "rn": resourceName
                }
            }
        else:
            body = {
                "m2m:cin": {
                    "cnf": "text/plain:0",
                    "con": content
                }
            }
        self.headers['Content-Type'] = "application/json;ty=4"
        return self.client.post(url, headers=self.headers, body=json.dumps(body))


class Container:
    def __init__(self, client, ae_base_name, resource_name):
        self.client = client
        self.url = os.path.join(ae_base_name, resource_name)

        response = self.client.retrieve(self.url)
        if response['code'] in [400, 404]:
            response = self.client.create_container(ae_base_name, resourceName=resource_name)
    
    def retrieve_latest(self):
        response = self.client.retrieve(os.path.join(self.url, "la"))
        return response

    def post(self, status):
        response = self.client.create_container_instance(self.url, content=status)
        return response


class RoboCar:
    def __init__(self):
        self.client = oneM2MClient(
            config.cseBaseName, 
            use_https=config.use_https,  
            originator=config.originator, 
            requestIdentifier=config.requestIdentifier,
            releaseVersionIndicator=config.releaseVersionIndicator)
        
        self.containers = {}
        self.containers['heartbeat'] = Container(self.client, config.aeBaseName, config.heartbeatContainer)
        self.containers['controller'] = Container(self.client, config.aeBaseName, config.controllerContainer)
        self.containers['traffic_light'] = Container(self.client, config.aeBaseName, config.trafficLightContainer)
        self.containers['stop_sign'] = Container(self.client, config.aeBaseName, config.stopSignContainer)

        self.status = "ok"
        self.latest_control_message_id = None
        self.get_controller()

    def send_heartbeat(self):
        logging.info(f"Sending heartbeat message: {self.status}")
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S:%f")
        response = self.containers['heartbeat'].post(f"RoboCar Client: {self.status} - {timestamp}")
        logging.info(f"Received heartbeat response: {response['code']} - {response['body']}")
        return response
    
    def get_controller(self):
        logging.info(f"Retrieving controller status.")
        response = self.containers['controller'].retrieve_latest()
        logging.info(f"Received controller response: {response['code']} - {response['body']}")
        body = response['body']
        if (response['code'] == 200) and body['m2m:cin']['ri'] != self.latest_control_message_id:
            self.latest_control_message_id = body['m2m:cin']['ri']
            return body['m2m:cin']['con']
        else:
            return None

    def send_traffic_light(self, status):
        logging.info(f"Sending traffic light status: {status}")
        response = self.containers['traffic_light'].post(status)
        logging.info(f"Received traffic light response: {response['code']} - {response['body']}")
        return response

    def send_stop_sign(self, status):
        logging.info(f"Sending stop sign status: {status}")
        response = self.containers['stop_sign'].post(status)
        logging.info(f"Received stop sign response: {response['code']} - {response['body']}")
        return response


def main():
    network_connected = False
    while not network_connected:
        try:
            logging.info(f"Connecting to the CSE server: {config.cseBaseName}")
            response = HTTPClient(config.cseBaseName, use_https=True).get()
        except socket.timeout as e:
            logging.warning(f"Network connection timeout: {e}")
            time.sleep(5)
        except http.client.HTTPException as e:
            logging.warning(f"HTTP request error: {e}")
            time.sleep(5)
        else:
            network_connected = True
            logging.info(f"RoboCar is able to reach the CSE server: {config.cseBaseName}")

    factory = PiGPIOFactory(host="localhost")
    led = LED(4)

    car = RoboCar()
    while True:
        car.send_heartbeat()
        controller = car.get_controller()
        if controller:
            led.on()
            time.sleep(config.retrieveDelay)
            led.off()
            print(controller)
            if controller == "q":
                car.status = "shutdown"
                car.send_heartbeat()
                break
        car.send_stop_sign("N/A")
        car.send_stop_sign("N/A")
        sys.stdout.flush()
        time.sleep(config.retrieveDelay)

if __name__ == "__main__":
    main()
