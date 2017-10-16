#!/usr/bin/env python
import rospy
from studentstudying.msg import Lab


def check():
    pub = rospy.Publisher('dispatcher_postgraduate', Lab, queue_size=20)
    rospy.init_node("AspirantAspirantov", anonymous=True)

    while not rospy.is_shutdown():
        code = raw_input("Hand over your homework! ")
        msg = Lab()
        msg.lab = code
        pub.publish(msg)

def student_write(labs):
    if labs.lab != "You are fired and in army hired!":
        rospy.loginfo("\n[Postgraduate talks] Labs from lazy student: " + labs.lab)
    else: 
        rospy.loginfo("\n[Postgraduate talks] OMG, he is in the army now")
        rospy.signal_shutdown("FAILED")

def dispatcher():
    rospy.wait_for_service('professor_service')
    student = rospy.Subscriber('dispatcher_studying_student', Lab, student_write)
    postgraduate = rospy.Subscriber('dispatcher_postgraduate', Lab, check)

    rospy.init_node('dispatcher', anonymous=True)
    rospy.spin()

if __name__ == '__main__':
    try:
        dispatcher()
    except rospy.ROSInterruptException:
        pass
