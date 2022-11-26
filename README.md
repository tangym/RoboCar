## RoboCar: 5G Cellular Remote Driving Robot Car
RoboCar is a remote-driving robot car using single-board computers combined with IoT oneM2M techniques.
The car can be controlled by a remote driver through the WiFi or 5G network and sends the camera view as well as the road status back to the remote user.  

About this project, you can find the source code in this [GitHub repository](https://github.com/tangym/RoboCar) and more information on the [project page](https://tangym.github.io/RoboCar/). We host a [web controller](https://acme-onem2m-cse-ix62mzt6pa-uc.a.run.app/webui/RoboCar.html) and [ACME server](https://acme-onem2m-cse-ix62mzt6pa-uc.a.run.app) which allows users to remote control RoboCar and check the container data on the server. 

![](https://i.imgur.com/O67vLMM.jpg)


## What You Need
RoboCar uses ARM-based development boards (Raspberry Pi 4 and NVIDIA Jetson Nano), 5G communication HAT, OpenCV, and other industry-standard components to show an applicable example of embedded systems. The system has the following components: motor car control,  live video streaming, 5G networking and oneM2M, road status recognition. 
The detail of each component is shown in the following table.

| Components                 | Hardware                                                                                                       | Software                         |
|:------------------------ |:-------------------------------------------------------------------------------------------------------------- |:-------------------------------- |
| Car control              | Raspberry Pi 4 model B <br/>Robot motor car <br/>TB6612FNG motor driver <br/>Prototyping board <br/>Jumper wires | GCC, Make                        |
| Live video streaming     | EMEET SmartCam C960 Webcam                                                                                     | Python 3.8+, OpenCV2, AWS        |
| 5G networking and oneM2M | SIM8200EA-M2 5G HAT, Straight Talk 5G SIM card                                                                 | Python 3.8+, SIMCOM driver, GCP  |
| Road status recognition  | NVIDIA Jetson Nano, Logitech C270 HD Webcam                                                                           | Python 3.8+, OpenCV2, TensorFlow |
   
## Links
- [GitHub Repository](https://github.com/tangym/RoboCar) 
- [Project Page](https://tangym.github.io/RoboCar/)
- [ACME CSE Server](https://acme-onem2m-cse-ix62mzt6pa-uc.a.run.app)
- [Controller WebUI](https://acme-onem2m-cse-ix62mzt6pa-uc.a.run.app/webui/RoboCar.html)
- [Live video streaming](http://rtmp-m2m.iamyourjun.com:8080/stat) and [RTMP](rtmp://rtmp-m2m.iamyourjun.com:8080/hls/stream.m3u8)

## Installation
### Raspberry Pi 5G HAT Setup
Follow the [SIM8200EA-M2 5G HAT - Waveshare Wiki](https://www.waveshare.com/wiki/SIM8200EA-M2_5G_HAT#Working_with_Raspberry_Pi) to setup your Raspberry Pi. 
- Download the latest Raspbian OS
- Run the following commands to install the driver for 5G HAT
```shell
sudo apt-get install p7zip-full
wget https://www.waveshare.com/w/upload/f/fb/SIM8200-M2_5G_HAT_code.7z
7z x SIM8200-M2_5G_HAT_code.7z
sudo chmod 777 -R SIM8200-M2_5G_HAT_code
cd SIM8200-M2_5G_HAT_code
sudo ./install.sh
```
- Test the 5G HAT is configured correctly with `AT` command
```shell
sudo apt-get install minicom
sudo minicom -D /dev/ttyUSB2
```
- Compile the `simcom` program
```shell
cd Goonline
make
```
- Enable 5G networking
```shell
sudo ./simcom-cm
```

### Raspberry Pi Setup
Download the source code from [RoboCar GitHub repository](https://github.com/tangym/RoboCar/) to your home directory `/home/pi`.
```Shell
git clone https://github.com/tangym/RoboCar.git
```

Run the `run.sh` command which checks if the GPIO daemon and 5G `simcom-cm` are running, and then starts oneM2M client and the motor control program.
```shell
./RoboCar/src/run.sh
```

To run the video streaming module, you need to install OpenCV.
```shell
sudo apt update
sudo apt install build-essential cmake pkg-config libjpeg-dev libtiff5-dev libjasper-dev libpng-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libfontconfig1-dev libcairo2-dev libgdk-pixbuf2.0-dev libpango1.0-dev libgtk2.0-dev libgtk-3-dev libatlas-base-dev gfortran libhdf5-dev libhdf5-serial-dev libhdf5-103 python3-pyqt5 python3-dev -y

pip3 install opencv-python==4.5.3.56
```
Then run the video streaming using the following command.
```shell
python3 camera.py
```

### ACME CSE Server Setup
Build a docker image of the ACME CSE server using the following command.
```shell
cd RoboCar/src/ACME-oneM2M-CSE/
docker build --no-cache -t acme-onem2m-cse -f tools/Docker/Dockerfile .
docker tag acme-onem2m-cse <my-container-registry>
docker push <my-container-registry>
```
And run the docker image on your server.
```shell
docker run -d -p 80:8080 acme-onem2m-cse 
```

### NVIDIA Jetson Nano Setup
Download the `car_detection` source code on NVIDIA Jetson Nano and run `python3 main.py`. The traffic light and stop sign indicators should be connected to GPIO 24 and 25 on Raspberry Pi.

## User Guide
You can check [RoboCar Live](http://rtmp-m2m.iamyourjun.com:8080/stat) for live streaming media information, and use media player (such as VLC) to open rtmp://rtmp-m2m.iamyourjun.com:8080/hls/stream.m3u8 to watch the live video streaming.

To control the RoboCar, go to [RoboCar Controller WebUI](https://acme-onem2m-cse-ix62mzt6pa-uc.a.run.app/webui/RoboCar.html) and click on the arrow buttons or press `A-W-D-X-S` on the keyboard.


