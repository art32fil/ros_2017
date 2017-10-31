#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
import rospy
from std_msgs.msg import String
import os
clear = lambda: os.system('clear')


array = [
     "########################################"
    ,"####### #####################     ######"
    ,"###### $ ## ################  # # ######"
    ,"####### ## ##### #########   #### ######"
    ,"####### ### #####  ###   #   #### ######"
    ,"##### # ######### ## # # #  ##### ## ###"
    ,"######  #######        #   ###### ######"
    ,"#######  ##  ### ### ##########  % #####"
    ,"######## ## #    ####### ###### ########"
    ,"########   ## ###### ########## ########"
    ,"##########    #   ############# ########"
    ,"########################################"
]


def draw_maze():
    clear()
    for line in array:
        print(line)


y_coord = [2]
x_coord = [7]
won = [False]


def callback(data):
    if won[0]:
        return
    data = str(data.data)
    y = y_coord[0]
    x = x_coord[0]
    line = list(array[y])
    line[x] = " "
    array[y] = "".join(line)

    if data == "UP":
        if y > 1 and array[y-1][x] != "#":
            y -= 1
    elif data == "DOWN":
        if y < 11 and array[y+1][x] != "#":
            y += 1
    elif data == "LEFT":
        if x > 1 and array[y][x-1] != "#":
            x -= 1
    else:
        if x < 41 and array[y][x+1] != "#":
            x += 1
    line = list(array[y])

    was_char = array[y][x]
    line[x] = "$"
    array[y] = "".join(line)
    
    y_coord[0] = y
    x_coord[0] = x
    draw_maze()

    if was_char == "%":
        won[0] = True
        print("YOU WON")


def listener():
    rospy.init_node('listener', anonymous=True)

    rospy.Subscriber("mazewalker", String, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
