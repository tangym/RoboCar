import os

if "raspberry" not in os.uname().version:
    debug = True
else:
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

logFile = "/home/pi/RoboCar.log" if not debug else "RoboCar.log"
retrieveDelay = 0.25 if not debug else 3

trafficLightPin = 10
stopSignPin = 11

motorControlProgram = "/home/pi/RoboCar/src/car_movement/hw0a1blikey2" if not debug else "python dummy_movement.py"