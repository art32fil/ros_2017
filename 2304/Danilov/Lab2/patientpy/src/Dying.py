#!/usr/bin/env python
import rospy
import sys
from patientpy.msg import Part
from patientpy.srv import *
from random import randint
from time import sleep

sicknesses = []
available_parts = ["Heart", "Lungs", "Eyes", "Hand", "Spine", "Legs", "Legs"]


def patient():
    pub = rospy.Publisher('dispatcher_dying_patient', Part, queue_size=20)
    srv = rospy.Service('doctor_service', Doctor, fix)

    rospy.init_node("johndoe", anonymous=True)

    while not rospy.is_shutdown():
        msg = Part()
        p = available_parts[randint(0, len(available_parts) - 1)]
        sicknesses.append(p)

        size = len(sicknesses)

        if size > 5:
            msg = Part()
            msg.part = "DEAD"
            pub.publish(msg)
            rospy.signal_shutdown("FAILED")

        if size > 3:
            p = "Hurry, we are loosing him, fix his " + p
        else:
            p = "We have a problem with his: " + p
        msg.part = p
        pub.publish(msg)
        sleep(randint(5, 15))


def fix(req):
    resp = DoctorResponse()
    for sickness in sicknesses:
        if (sickness == req.whichPart):
            sicknesses.remove(sickness)
            resp.result = "We Fixed It"
            return resp
    resp.result = "Wrong"
    return resp

if __name__ == '__main__':
    try:
        patient()
    except rospy.ROSInterruptException:
        pass
