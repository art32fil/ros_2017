#!/usr/bin/env python
import rospy
import math
from std_msgs.msg import String
from random import randint

id = "robot0"
publisher = ""

def stupidAI():
    global id
    rospy.init_node('stupidAI')
    id = rospy.get_param('~id')

    global publisher
    publisher = rospy.Publisher('inputs', String, queue_size=20)

    rospy.sleep(10)
    while not (rospy.is_shutdown()):
        if randint(-4, 4) > 2:
            publisher.publish(id + " 0 1")
        else:
            publisher.publish(id + " 1 0")

        rospy.sleep(0.01)


if __name__ == '__main__':
    try:
        stupidAI()
    except rospy.ROSInterruptException: pass