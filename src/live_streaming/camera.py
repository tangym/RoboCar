import time

import cv2
import os
import sys
import subprocess


class CameraHandler:
    def __init__(self,
                 inputDevice=0,
                 inputDevice_fps=30,
                 inputDevice_width=640,
                 inputDevice_hight=480,
                 rtmpUrl="rtmp://rtmp-m2m.iamyourjun.com/live/stream",
                 **kwargs):

        #Initializer

        #-y : Overwrite output files without asking.
        #-f : Force input or output file format. The format is normally auto detected for input files and guessed from the file extension for output files, so this option is not needed in most cases.
        #-vcodec : Set the video codec. This is an alias for -codec:v.
        #-pix_fmt : Show available pixel formats.
        #-r : Set frame rate (Hz value, fraction or abbreviation).


        self.inputDevice_fps = None
        self.inputDevice_frame_width = None
        self.inputDevice_frame_height = None
        self.rtmpUrl = "rtmp://rtmp-m2m.iamyourjun.com/live/stream"

        self.inputDevice = inputDevice





    def activateCapture(self):
        try:
            self.__capture = cv2.VideoCapture(self.inputDevice)
            self.__capture.set(cv2.CAP_PROP_BUFFERSIZE, 1)

            self.inputDevice_fps = int(30)
            self.inputDevice_frame_width = 640
            self.inputDevice_frame_height = 480

            self.__capture.open(self.inputDevice)

            print("Capture Activated")
        except Exception as e:
            raise RuntimeError("Device N/A"+e)

    def deactivateCapture(self):
        try:
            self.__capture.release()
        except:
            raise IOError("Device Release Failed")

    def activateStream(self):
        self.__ffmpeg_command = ['ffmpeg',
           '-y',
           '-f', 'rawvideo',
           '-vcodec', 'rawvideo',
           '-pix_fmt', 'bgr24',
           '-s', "{}x{}".format(self.inputDevice_frame_width, self.inputDevice_frame_height),
           '-r', str(self.inputDevice_fps),
            '-rtbufsize','400k',
           '-i', '-',
           '-c:v', 'libx264',
            '-vf','scale=640:480',
           '-pix_fmt', 'yuv420p',
           '-preset', 'ultrafast',
           '-b:v','500k',
           '-tune','zerolatency',
           '-maxrate','800k',
                                 '-bufsize', '300k',
           '-f', 'flv',
           self.rtmpUrl]

        try:
            p = subprocess.Popen(self.__ffmpeg_command, stdin=subprocess.PIPE)

            while True:
                ret, frame = self.__capture.read()
                if not ret:
                    raise IOError("No Capture Return")

                p.stdin.write(frame.tobytes())

        except Exception as e:
            raise IOError(e)

    def deactivateStream(self):
        try:
            self.out.release()
        except:
            raise IOError("DeActivate Stream Failed")

    def startTest(self):
        try:
            while True:
                self.ret, self.frame = self.__capture.read()
                cv2.imshow("Press q to exit",self.frame)
                if not self.ret:
                    raise IOError("No Frame(RET)")

                if (cv2.waitKey(1) & 0xFF == ord('q')):
                    break

        except:
            raise UnboundLocalError("Video Screen activation failed")

    def stopTest(self):
            try:
                cv2.destroyAllWindows()
                return True

            except:
                raise UnboundLocalError("Video Screen deactivation failed")

if __name__ == '__main__':

    cm = CameraHandler()
    cm.activateCapture()
    cm.activateStream()
