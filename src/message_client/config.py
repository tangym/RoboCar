debug = False
cseBaseName = "acme-onem2m-cse-ix62mzt6pa-uc.a.run.app"    # <== Put your own ACME CSE server here
use_https = True
encoding = "ISO-8859-1"
requestIdentifier = "RoboCarClient"
controllerContainer = "control"
heartbeatContainer = "RoboCarClientHeartBeat"
trafficLightContainer = "trafficLight"
stopSignContainer = "stopSign"
shellOutputContainer = "shellOutput"
originator = "CAdmin" # "RoboCar" 
releaseVersionIndicator = "3"
aeBaseName = "/cse-in/CAdmin"
logFile = "RoboCar.log" # "/home/pi/RoboCar.log"
if debug:
    retrieveDelay = 3
else:
    retrieveDelay = 0.25
trafficLightPin = 10
stopSignPin = 11
motorControlProgram = "/home/pi/RoboCar/src/car_movement/hw0a1blikey2"  # or "python dummy_movement.py"