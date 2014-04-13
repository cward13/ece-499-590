#!/usr/bin/env python
# /* -*-  indent-tabs-mode:t; tab-width: 8; c-basic-offset: 8  -*- */
# /*
# Copyright (c) 2014, Daniel M. Lofaro <dan (at) danLofaro (dot) com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the author nor the names of its contributors may
#       be used to endorse or promote products derived from this software
#       without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# */
import diff_drive
import ach
import sys
import time
from ctypes import *
import socket
import cv2.cv as cv
import cv2
import numpy as np

dd = diff_drive
ref = dd.H_REF()
tim = dd.H_TIME()

ROBOT_DIFF_DRIVE_CHAN   = 'robot-diff-drive'
ROBOT_CHAN_VIEW   = 'robot-vid-chan'
ROBOT_TIME_CHAN  = 'robot-time'
# CV setup 
cv.NamedWindow("wctrl", cv.CV_WINDOW_AUTOSIZE)
#capture = cv.CaptureFromCAM(0)
#capture = cv2.VideoCapture(0)
##sock.connect((MCAST_GRP, MCAST_PORT))
newx = 320
newy = 240

nx = 640
ny = 480

r = ach.Channel(ROBOT_DIFF_DRIVE_CHAN)
r.flush()
v = ach.Channel(ROBOT_CHAN_VIEW)
v.flush()
t = ach.Channel(ROBOT_TIME_CHAN)
t.flush()

i=0


print '======================================'
print '============= Robot-View ============='
print '========== Daniel M. Lofaro =========='
print '========= dan@danLofaro.com =========='
print '======================================'
while True:
    # Get Frame
    img = np.zeros((newx,newy,3), np.uint8)
    c_image = img.copy()
    vid = cv2.resize(c_image,(newx,newy))
    [status, framesize] = v.get(vid, wait=False, last=True)
    if status == ach.ACH_OK or status == ach.ACH_MISSED_FRAME or status == ach.ACH_STALE_FRAMES:
        vid2 = cv2.resize(vid,(nx,ny))
        img = cv2.cvtColor(vid2,cv2.COLOR_BGR2RGB)
        cv2.imshow("wctrl", img)
        cv2.waitKey(10)
    else:
        raise ach.AchException( v.result_string(status) )


    [status, framesize] = t.get(tim, wait=False, last=True)
    if status == ach.ACH_OK or status == ach.ACH_MISSED_FRAME or status == ach.ACH_STALE_FRAMES:
        pass
        #print 'Sim Time = ', tim.sim[0]
    else:
        raise ach.AchException( v.result_string(status) )

#-----------------------------------------------------
#--------[ Do not edit above ]------------------------
#-----------------------------------------------------
    # Name: Colin Ward
    # ref.ref[0] = Right Wheel Velos
    # ref.ref[1] = Left Wheel Velos
    # tim.sim[0] = Sim Time
    # img        = cv image in BGR format
    #Here we are defining all the colors that we will run into and cutting all the values we dont want. 
    #We are also timing the process and waiting if its under the desired update rate
    otime2=tim.sim[0]
    err=0
    if(i==0):
	starttime=tim.sim[0]
	f = open("data2.txt", "w")
    i=i+1
    blue = cv2.inRange(img, np.array([0,0,0], dtype = np.uint8), np.array([255,0,0], dtype = np.uint8));
    cv2.namedWindow("Color Vision", cv2.WINDOW_AUTOSIZE);
    cv2.imshow("Color Vision", blue);
    #Below sets the color we are searching for
    colorsearch=blue
    #Finds Objects in view with the color chosen above
    cntRGB, h = cv2.findContours(colorsearch, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    #Check determines whether the object of what we are looking for comes into FOV
    check=0
    #Below finds the center of the object in question and puts a circle around the object
    #(although its a rectangle) and sets check to 1 so the motors will begin homing in on the center
    for cnt in cntRGB:
	(x, y), radius = cv2.minEnclosingCircle(cnt)
	center = (int(x), int(y))
	radius = int(radius)
	check=1
	print 'x, y ', x, y
    #When the object is in the Field of view the robot will find the center of the videoframes distance from the center of
    #the square and adjust the robots position based on this distance
    if(check==1):
	#Below will find the distance in frame size and stores it in err
    	err = (nx/2) - x;
	#Below is simply tells the user what the distance is
    	print 'error in pixels = ',err
	print 'error in percent = ', err/640
	#Now a gain is applied to the error of the given system and fed to the servo-motor values
    	ref.ref[0]=err*.012
	ref.ref[1]=-1*err*.012
    #If the object is not in FOV the robot will continue searching
    else:
	ref.ref[0] = -0.4
    	ref.ref[1] = 0.4
    #return values to user
    print 'Sim Time = ', tim.sim[0]
    print 'Clock time=', time.clock
    print 'Engine Values', ref.ref[0],ref.ref[1]
    print i
    #[status, framesize] = t.get(tim, wait=False, last=True)
    #timer=tim.sim[0]-starttime
    #f.write( str(timer)  )
    #f.write("\n")  
    # Sets reference to robot
    r.put(ref);

    # Sleeps
    #Sets frequency to 20Hz even if the code executes faster
    while((tim.sim[0]-otime2)<.05):
	[status, framesize] = t.get(tim, wait=False, last=True)
    timer=tim.sim[0]-otime2
    #prints the time elapsed and the error to a txt document
    timeelapsed=tim.sim[0]-starttime
    f.write( str(err)  )
    f.write(" ")
    f.write( str(timeelapsed))
    f.write("\n")  
#-----------------------------------------------------
#--------[ Do not edit below ]------------------------
#-----------------------------------------------------
