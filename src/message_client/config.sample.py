debug = True
cseBaseName = "==>put your own ACME CSE server here<=="
use_https = True
encoding = "ISO-8859-1"
requestIdentifier = "identifier"
controllerContainer = "control"
heartbeatContainer = "RoboCarClientHeartBeat"
trafficLightContainer = "trafficLight"
stopSignContainer = "stopSign"
originator = "CAdmin"
releaseVersionIndicator = "3"
aeBaseName = "/cse-in/CAdmin"
if debug:
    retrieveDelay = 1
else:
    retrieveDelay = 0.1