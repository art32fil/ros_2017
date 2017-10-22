#!/usr/bin/env python

import rospy
from patientpy.msg import *
from patientpy.srv import *


def nursery_talk(data):
    if data.part != "DEAD":
        rospy.loginfo("\n[Nurse] Call from ambulatory: " + data.part)
    else:
        rospy.loginfo("\n[Nurse] PATIENT IS DEAD")
        rospy.signal_shutdown("FAILED")


def intern_talk(data):
    try:
        doctorService = rospy.ServiceProxy('doctor_service', Doctor)
        result = doctorService(data.part)
        if (result.result == "Wrong"):
            rospy.loginfo("[Nurse] That's wrong!")
        else:
            rospy.loginfo("[Nurse] Thank you, " + result.result)
    except rospy.ServiceException, e:
        rospy.logerr("Service call failed: %s" % e)


def nurse():
    rospy.wait_for_service('doctor_service')
    patient = rospy.Subscriber('dispatcher_dying_patient', Part, nursery_talk)
    intern = rospy.Subscriber('dispatcher_intern', Part, intern_talk)

    rospy.init_node('nurse', anonymous=True)
    rospy.spin()


if __name__ == '__main__':
    nurse()
