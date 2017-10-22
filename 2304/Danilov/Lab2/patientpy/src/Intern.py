#!/usr/bin/env python
import rospy
from patientpy.msg import Part


def nursery_talk(data):
    if data.part == "DEAD":
        rospy.loginfo("\n[Intern] OMG, I killed HIM!!")
        rospy.signal_shutdown("FAILED")

def intern():
    pub = rospy.Publisher('dispatcher_intern', Part, queue_size=20)
    patient = rospy.Subscriber('dispatcher_dying_patient', Part, nursery_talk)
    rospy.init_node("johndoe", anonymous=True)

    while not rospy.is_shutdown():
        code = raw_input("Which patients part to treat?: ")
        msg = Part()
        msg.part = code
        pub.publish(msg)


if __name__ == '__main__':
    try:
        intern()
    except rospy.ROSInterruptException:
        pass
