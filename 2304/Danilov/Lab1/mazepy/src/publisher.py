#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
from std_msgs.msg import String
import sys, os


def wait_key():
    ''' Wait for a key press on the console and return it. '''
    result = None
    if os.name == 'nt':
        import msvcrt
        result = msvcrt.getch()
    else:
        import termios
        fd = sys.stdin.fileno()

        oldterm = termios.tcgetattr(fd)
        newattr = termios.tcgetattr(fd)
        newattr[3] = newattr[3] & ~termios.ICANON & ~termios.ECHO
        termios.tcsetattr(fd, termios.TCSANOW, newattr)

        try:
            result = sys.stdin.read(1)
        except IOError:
            pass
        finally:
            termios.tcsetattr(fd, termios.TCSAFLUSH, oldterm)

    return result

def mazewalker():
    pub = rospy.Publisher('mazewalker', String, queue_size=10)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    print("You are an $ looking for it's (best) %. Use AWSD to get to it!")
    while not rospy.is_shutdown():
        butt = wait_key()
        if butt == "a":
            msg = "LEFT"
        elif butt == "w":
            msg = "UP"
        elif butt == "s":
            msg = "DOWN"
        else:
            msg = "RIGHT"
        pub.publish(msg)

if __name__ == '__main__':
    try:
        mazewalker()
    except rospy.ROSInterruptException:
        pass
