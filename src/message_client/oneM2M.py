import http.client
import json
import logging


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
