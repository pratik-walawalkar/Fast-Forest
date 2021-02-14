# -*- coding: utf-8 -*-
"""
Created on Fri Feb 12 17:47:13 2021

@author: Pratik
"""

import time
#import clr #pythonnet, manually installed with a downloaded wheel and pip
import ctypes #module to open dll files
#clr.AddReference('EposCmd.Net')
EposCmd64 = ctypes.CDLL('.\EposCmd64.dll')
from EposCmd.Net.VcsWrapper import Device

# configure desired motion profile in the lines below
velocity = 83            #rpm profile speed
acceleration = 3e5       #rpm/s, up to 1e7 would be possible
deceleration = 3e5       #rpm/s
number_of_cycles = 100
angle = 40               #degrees
delay = angle/6/velocity #delay in seconds. This determines the angle to be reached, approximately

home = 0                 #start position
target = 100000          #put in a big number, bigger than the desired end position
trigger = 50             #the value after which the state is changed
sensor_input = 0         #input from the sensor
engine_on = True

#---------------------------------------------------------------------------------------------------------------------------------------

#---------------------------------------------------------------------------------------------------------------------------------------


Device.Init()
nodeID = 1
baudrate = 1000000
timeout = 500
errorCode = 0
absolute = True         #as opposed to relative
immediately = True      #does not wait for execution of previous command
timewait = 2000         #maxtime for position reach


keyHandle, error = Device.VcsOpenDevice('EPOS4', 'MAXON SERIAL V2', 'USB', 'USB0', errorCode) #opens EPOS
Device.VcsSetProtocolStackSettings(keyHandle, baudrate, timeout, errorCode)                   #set baudrate
Device.VcsClearFault(keyHandle, nodeID, errorCode)                                            #clear all faults
Device.VcsActivateProfilePositionMode(keyHandle, nodeID, errorCode)                           #activate profile position mode
Device.VcsSetPositionProfile(keyHandle, nodeID, velocity, acceleration, deceleration, errorCode) #set profile parameters
Device.VcsSetEnableState(keyHandle, nodeID, errorCode)                                        #enable device
  
#go to initial position before start
Device.VcsMoveToPosition(keyHandle, nodeID, home, True, True, errorCode)
Device.VcsWaitForTargetReached(keyHandle, nodeID, timewait, errorCode)

while engine_on:
    if sensor_input <= trigger:
        Device.VcsMoveToPosition(keyHandle, nodeID, target, absolute, immediately, errorCode)
        time.sleep(delay)
    
    else:
        Device.VcsMoveToPosition(keyHandle, nodeID, home, absolute, immediately, errorCode)
        time.sleep(delay)  


