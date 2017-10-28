#!/usr/bin/env python
import rospy
from lab_2.msg import *
from lab_2.srv import *

def callback(data):
    if data.message == "Failed!":
        rospy.loginfo("\nThe exam is failed! \nEnter any key...")
        rospy.signal_shutdown("Failed!")
    else:
        rospy.loginfo(data.message)

def student():
    rospy.wait_for_service('exam_service')
    patient = rospy.Subscriber('exam', Message, callback)
    rospy.init_node("Vasya", anonymous=True)

    rospy.loginfo("\nCalculate following expressions to pass exam:\n")

    while not rospy.is_shutdown():
        answer = raw_input()

        if not rospy.is_shutdown():
            try:
                examService = rospy.ServiceProxy('exam_service', Exam)
                result = examService(answer)
                rospy.loginfo(result.result)
            except rospy.ServiceException, e:
                rospy.logerr("Service call failed: %s" % e)

if __name__ == '__main__':
    try:
        student()
    except rospy.ROSInterruptException:
        pass
