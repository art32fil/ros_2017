#!/usr/bin/env python
import rospy
import sys
from studentstudying.msg import Lab
from studentstudying.srv import *
from random import choice
from random import randint
from string import ascii_uppercase
from time import sleep

codes = []


def check(studentRequest):
    profResponse = ProfessorResponse()
    for code in codes:
        if (code == studentRequest.lab):
            codes.remove(code)
            profResponse.mark = "You passed it"
            return profResponse
    profResponse.mark = "You have mistakes in your lab, do it again, lazy hobo!"
    return profResponse


def publisher():
    pub = rospy.Publisher('dispatcher_studying_student', Lab, queue_size=20)
    srv = rospy.Service('professor_service', Professor, check)

    rospy.init_node("PetrKozlov", anonymous=True)

    messages = ["ROS", "ParallelProgramming", "English", "RWS", "Graduate Work", "Physics"]
    while not rospy.is_shutdown():

        sleep(randint(5, 12))
        msg = Lab()
        p = messages[randint(0, 5)]
        codes.append(p)

        size = len(codes)

        if size > 4:
            msg = Lab()
            msg.lab = "You are fired and in army hired!"
            pub.publish(msg)
            rospy.signal_shutdown("FAILED")

        p = "Hey, lazy lizard, do some homework, quickly!!!! " + p

        msg.lab = p
        pub.publish(msg)
        sleep(randint(15, 30))


if __name__ == '__main__':
    try:
        publisher()
    except rospy.ROSInterruptException:
        pass
