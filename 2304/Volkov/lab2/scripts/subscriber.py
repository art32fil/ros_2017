#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
import rospy
from std_msgs.msg import String
from ussr.srv import *
import os
clear = lambda: os.system('clear')

####################
# GLOBAL VARIABLES #
####################
countryList = ["Kazakhstan", "Ukraine", "Belarus", "Uzbekistan", "Georgia", "Azerbaijan", "Lithuania", "Moldavia", "Latvia", "Kirghizia", "Tajikistan", "Armenia", "Turkmenia", "Estonia"]
countCountry = len(countryList)
countI = 0
saveCountry = 0
win = [
    "█─█───███───███───████",
    "█─█───█─────█─────█──█",
    "█─█───███───███───████",
    "█─█─────█─────█───█─█",
    "███─█─███─█─███─█─█─█──█",
    ""
    ""
    "███─█───█  ████──████─████─█──█",
    "─█──██─██  █──██─█──█─█──█─█─█",
    "─█──█─█─█  ████──████─█────██ ",
    "─█──█───█  █──██─█──█─█──█─█─█  " ,
    "███─█───█  ████──█──█─████─█──█ " ,
    ""
    ""
    "███─█──█  ███─█──█─███  █─█───███───███───████",
    "─█──██─█  ─█──█──█─█    █─█───█─────█─────█──█",
    "─█──█─██  ─█──████─███  █─█───███───███───████",
    "─█──█──█  ─█──█──█─█    █─█─────█─────█───█─█",
    "███─█──█  ─█──█──█─███  ███─█─███─█─███─█─█─█──█"
]

def draw_maze():
    for line in win:
        print(line)
####################
# SERVICE - HANDLE #
####################
def handle_save_country(req):
    global saveCountry
    if saveCountry > 1:
        saveCountry = saveCountry - 1# SAVE COUNTRY
    #rospy.loginfo("%s is saved", req.request)
    return AddTwoIntsResponse(countryList[countI])# SERVICE - SEND

#################
# MAIN FUNCTION #
#################
def subscriber():
    rospy.init_node('publisher', anonymous=True)# TOPIC - INIT
    pub = rospy.Publisher('newspaper', String, queue_size=10)# TOPIC - CONNECT
    s = rospy.Service('save_country', AddTwoInts, handle_save_country)# SERVICE - CONNECT

    global countI
    global countCountry
    global saveCountry


    rate = rospy.Rate(0.5)#
    finish = countCountry-1
    while (not rospy.is_shutdown()) and (countI < finish):
        rospy.loginfo("Type 's' to save %s", countryList[countI+1])
        msg = countryList[countI]# TOPIC - message
        
        if saveCountry > countCountry/2:# Check if we separated countries less than union
            rospy.loginfo("USSR is destroyed and doesn't exist anymore %s", countI)
            return

        pub.publish(msg)# TOPIC - SEND
        countI = countI + 1# WHILE INCREMENT
        saveCountry = saveCountry + 1
        rate.sleep()# TIME WAIT
    rospy.loginfo("If you see this message, you've saved the Soviet Union")
    draw_maze()
    #rospy.init_node('save_country')

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

#################
# PROGRAM START #
#################
if __name__ == '__main__':
    subscriber()
