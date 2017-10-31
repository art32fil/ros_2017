#!/usr/bin/env python
from __future__ import print_function
import rospy
from std_msgs.msg import String

import os
import sys
clear = lambda: os.system('clear')

level = [
    1, 1, 1, 1, 1,
    1, 0, 0, 2, 1,
    1, 0, 1, 0, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1
]

size = 5

ux = 1
uy = 1


def draw_maze():
    clear()
    y = 0

    global ux
    global uy

    while y < size:
        x = 0
        line = ""
        while x < size:
            if x == ux and y == uy:
                line += '@'
                if level[y * size + x] == 2:
                    ux = 1;
                    uy = 1;
                    draw_maze()
                    return
            elif level[y * size + x] == 1:
                line += '#'
            elif level[y * size + x] == 2:
                line += 'F'
            else:
                line += ' '
            x=x+1
        print (line)
        y=y+1


def free(cell):
    if level[cell] == 0: return True
    if level[cell] == 2: return True
    return False


def callback(data):

    global ux, uy
    msg = data.data
    if msg == "up":
        if free((uy+1) * size + ux):
            uy+=1
    elif msg == "down":
        if free((uy-1) * size + ux):
            uy-=1
    elif msg == "left":
        if free((uy * size) + (ux-1)):
            ux-=1
    elif msg == "right":
        if free((uy * size) + (ux+1)):
            ux=ux+1

    draw_maze()

def listener():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("chatter", String, callback)
    rospy.spin()


if __name__ == '__main__':
    draw_maze()
    listener()