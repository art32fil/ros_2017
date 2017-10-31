#!/usr/bin/env python
import roslib; roslib.load_manifest('lab1')
import rospy
import sys

from std_msgs.msg import String

def talker():
    pub = rospy.Publisher('chatter', String, queue_size=10)
    rospy.init_node('talker')

    exit = False
    while not (exit or rospy.is_shutdown()):
        key = sys.stdin.read(1)
        str = ''

        if key == 'a' or key == 'A':
            #Left
            str = "left"
        elif key == 'd' or key == 'D':
            #Right
            str = "right"
        elif key == 'w' or key == 'W':
            #Up
            str = "down"
        elif key == 's' or key == 'S':
            #Down
            str = "up"
        elif key == 'e' or key == 'E':
            #Exit
            exit = True

        if str != '' :
            pub.publish(String(str))
        rospy.sleep(1.0)


if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException: pass