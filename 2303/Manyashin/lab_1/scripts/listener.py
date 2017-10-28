#!/usr/bin/env python

import rospy
from std_msgs.msg import String
import random
import copy

topik = "lab_1"

class Robot:
    def __init__(self,map = None, name="R2D2", x=2, y=2):
        self.name = name
        self.x = x
        self.y = y
        self.map = map

    def move(self, dx, dy):
        new_x = self.x + dx
        new_y = self.y + dy
        if self.is_free(new_x, new_y):
            rospy.loginfo('%s move to (%d,%d)', self.name,new_x, new_y)
            self.x = new_x
            self.y = new_y
        else:
            rospy.loginfo('%s move on wall' % self.name)

    def show_location(self):
        map = copy.deepcopy(self.map)
        
        map[self.y][self.x] = "*"
        for i in map:
            print ', '.join(str(v) for v in i)
        print ""
        del map

    def is_free(self,x,y):
        if (x >= 0 and x < len(self.map[0]) and y >= 0 and y < len(self.map)):
            return self.map[y][x] == 1 
        else:
            return False

    def run_on(self, topic):
        def run_callback(data):
            if data.data == 'up':
                self.move(0, -1)

            if data.data == 'left':
                self.move(-1, 0)

            if data.data == 'right':
                self.move(1, 0)

            if data.data == 'down':
                self.move(0, 1)

            if data.data == 'print':
                self.show_location()

        rospy.init_node('listener', anonymous=True)
        rospy.Subscriber(topic, String, run_callback)
        rospy.spin()


def callback(data):
    rospy.loginfo('%s - get data %s' % (rospy.get_time(),data.data))


def genarate_map(mx=5,my=5):
    # 1 is free
    # 0 is not free

    nmap = []
    imgx = mx * 1
    imgy = my * 1

    maze = [[0 for x in range(mx)] for y in range(my)]
    dx = [0, 1, 0, -1]
    dy = [-1, 0, 1, 0]
    stack = [(random.randint(0, mx - 1), random.randint(0, my - 1))]

    while len(stack) > 0:
        (cx, cy) = stack[-1]
        maze[cy][cx] = 1
        # find a new cell to add
        nlst = []  # list of available neighbors
        for i in range(4):
            nx = cx + dx[i]
            ny = cy + dy[i]
            if nx >= 0 and nx < mx and ny >= 0 and ny < my:
                if maze[ny][nx] == 0:
                    # of occupied neighbors must be 1
                    ctr = 0
                    for j in range(4):
                        ex = nx + dx[j];
                        ey = ny + dy[j]
                        if ex >= 0 and ex < mx and ey >= 0 and ey < my:
                            if maze[ey][ex] == 1: ctr += 1
                    if ctr == 1: nlst.append(i)
        # if 1 or more neighbors available then randomly select one and move
        if len(nlst) > 0:
            ir = nlst[random.randint(0, len(nlst) - 1)]
            cx += dx[ir];
            cy += dy[ir]
            stack.append((cx, cy))
        else:
            stack.pop()

    # paint the maze
    for ky in range(imgy):
        line = []
        for kx in range(imgx):
            value = maze[my * ky / imgy][mx * kx / imgx]
            line.append(value)
        nmap.append(line)
    return nmap



def listener():

    mrobot = Robot(genarate_map())

    mrobot.run_on(topik)

    #rospy.init_node('listener', anonymous=True)

    #rospy.Subscriber(topik, String, callback)

    #rospy.spin()

if __name__ == '__main__':
    listener()
