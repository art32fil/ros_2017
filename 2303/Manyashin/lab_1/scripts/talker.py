#!/usr/bin/env python

import rospy
from std_msgs.msg import String
import random


def generate_key():
    key = random.randint(1,6)
    if key == 1: return "up"
    if key == 2: return "down"
    if key == 3: return "left"
    if key == 4: return "right"
    if key == 5: return "print"

def talker():
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    topik = "lab_1"
    pub = rospy.Publisher(topik, String, queue_size=1000)

    while not rospy.is_shutdown():

        key = generate_key()
        rospy.loginfo("%s - Generate key %s" % (rospy.get_time(),key))
        pub.publish(key)
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
