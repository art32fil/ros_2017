#!/usr/bin/env python
import sys
import rospy
from lab2.msg import *
from lab2.srv import *

service = rospy.ServiceProxy('deep_thought_service', Calculator)

def new_input(data):
    if data.data != "BSOD":
        print data.data
    else:
        print "Deep Thought shotdown"
        rospy.signal_shutdown("FAILED")

def new_question(data):
    print data.data
    try:
        sum = input("Sum: ")
        ans = int(sum)
    except NameError:
        print "The universe is hacked"
        ans = 42
    result = service(ans)
    print result
    if result.result == "INCORRECT":
        new_question(data)


def processor_thread():
    print "Processor started"
    rospy.wait_for_service('deep_thought_service')
    print "Processor connected"
    pub = rospy.Publisher('deep_thought_databus', NotString, queue_size=20)
    rospy.init_node('processor', anonymous=True)
    rospy.sleep(1.0)
    pub.publish("GO!")
    print "Processor send signal"
    rospy.Subscriber('deep_thought_process', NotString, new_input)
    rospy.Subscriber('deep_thought_question', NotString, new_question)
    rospy.spin()


if __name__ == "__main__":
    try:
        processor_thread()
    except rospy, e:
        rospy.logerr("Procesor call failed: %s" % e)