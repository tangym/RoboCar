import http.client
import json

import config


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
        return response.read().decode(config.encoding)

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
        return self.client.get(url, headers=self.headers)

    def delete(self, url):
        return self.client.delete(url, headers=self.headers)
    
    def update(self, url, body):
        return self.client.put(url, headers=self.headers, body=body)
    
    def create(self, url, body):
        return self.client.post(url, headers=self.headers, body=body)
    
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
                "mbs": 10000,
                "mni": 10,
                "rn": resourceName
            }
        }
        self.headers['Content-Type'] = "application/json;ty=3"
        return self.client.post(url, headers=self.headers, body=json.dumps(body))





# client = HTTPClient(config.cseBaseName)
# print(client.get("cse-in"))

client = oneM2MClient(
    config.cseBaseName, 
    use_https=config.use_https,  
    originator=config.originator, 
    requestIdentifier=config.requestIdentifier,
    releaseVersionIndicator=config.releaseVersionIndicator)

response = client.retrieve("/cse-in")
# response = client.create_application_entity("/cse-in", resourceName="Notebook-AE", appID="NnotebookAE", requestReachability=True)
# response = client.create_application_entity("/cse-in/CAdmin", resourceName="myContainer")
print(response)
