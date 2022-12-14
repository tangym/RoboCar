import io
import os
import sys
import time
import socket
import http.client
from datetime import datetime
import logging
import subprocess
import threading
from queue import Queue, Empty
import shlex

if "raspberry" in os.uname().version:
    from gpiozero import LED, PWMLED, Button
    from gpiozero.pins.pigpio import PiGPIOFactory

import config
from oneM2M import HTTPClient, oneM2MClient, Container


if config.debug:
    logging.basicConfig(filename=config.logFile, level=logging.DEBUG)
else:
    logging.basicConfig(filename=config.logFile, level=logging.INFO)

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
        self.containers['shell_output'] = Container(self.client, config.aeBaseName, config.shellOutputContainer)

        self.status = "ok"
        self.latest_control_message_id = None
        self.get_controller()
        self.motor_process = InteractiveProcess(config.motorControlProgram)

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
    
    def execute(self, command):
        if command in list("adwxsij"):
            self.motor_process.write_stdin(command + "\n")
            output = self.motor_process.read_stdout()
        else:
            logging.info(f"Executing command: {command}")
            process = InteractiveProcess(command)
            output = process.read_stdout()
        logging.info(f"Posting execution output: {output}")
        response = self.containers['shell_output'].post(output)
        logging.info(f"Received response: {response['code']} - {response['body']}")
        return response


class InteractiveProcess:
    def __init__(self, command, read_stdout_timeout=0.1):
        if type(command) is str:
            command = shlex.split(command)
        self.process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, encoding="utf-8")
        self.stdout = Queue()
        self.read_stdout_timeout = read_stdout_timeout
        writer = threading.Thread(target=self._poll_stdout)
        writer.daemon = True
        writer.start()

    def _poll_stdout(self):
        for line in iter(self.process.stdout.readline, ''):
            self.stdout.put(line)
        self.process.stdout.close()

    def read_stdout(self):
        output = []
        while True:
            try:
                line = self.stdout.get(timeout=self.read_stdout_timeout)
                output.append(line)
            except Empty:
                break
        return "".join(output)

    def write_stdin(self, command):
        self.process.stdin.write(command)
        self.process.stdin.flush()

    def poll(self):
        return self.process.poll()


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

    if "raspberry" in os.uname().version:
        factory = PiGPIOFactory(host="localhost")

    car = RoboCar()
    if "raspberry" in os.uname().version:
        traffic_light = Button(config.trafficLightPin)
        stop_sign = Button(config.stopSignPin)
    while True:
        car.send_heartbeat()
        controller = car.get_controller()
        if controller:
            if controller == "q":
                car.status = "shutdown"
                car.send_heartbeat()
                break
            else:  # assume receiving a bash command
                car.execute(controller)
        sys.stdout.flush()
        if "raspberry" in os.uname().version:
            car.send_traffic_light(str(not traffic_light.is_pressed))
            car.send_stop_sign(str(not stop_sign.is_pressed))
        time.sleep(config.retrieveDelay)


if __name__ == "__main__":
    main()
