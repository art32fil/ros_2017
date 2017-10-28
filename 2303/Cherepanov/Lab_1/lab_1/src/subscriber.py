#!/usr/bin/env python

import turtle
import rospy
import pyautogui
from std_msgs.msg import String

class Pen(turtle.Turtle):
    def __init__(self):
        turtle.Turtle.__init__(self)
        self.shape('square')
        self.color('white')
        self.penup()
        self.speed(0)

class Player(turtle.Turtle):
    def __init__(self):
        turtle.Turtle.__init__(self)
        self.shape('square')
        self.color('blue')
        self.penup()
        self.speed(0)

    def go_up(self):
        new_x = self.xcor()
        new_y = self.ycor() + 24
        if (new_x, new_y) not in walls:
            self.goto(new_x, new_y)
            rospy.loginfo('I go up')

    def go_down(self):
        new_x = self.xcor()
        new_y = self.ycor() - 24
        if (new_x, new_y) not in walls:
            self.goto(new_x, new_y)
            rospy.loginfo('I go down')

    def go_left(self):
        new_x = self.xcor() - 24
        new_y = self.ycor()
        if (new_x, new_y) not in walls:
            self.goto(new_x, new_y)
            rospy.loginfo('I go left')

    def go_right(self):
        new_x = self.xcor() + 24
        new_y = self.ycor()
        if (new_x, new_y) not in walls:
            self.goto(new_x, new_y)
            rospy.loginfo('I go right')

level = [
"XXXXXXXXXXXXXXXXXXXXXXXXX",
"XP XXXXXXXX         XXXXX",
"X  XXXXXXXX  XXXXX  XXXXX",
"X      XXXX  XXXXX  XXXXX",
"X        XX  XX        XX",
"XXXXXXX  XX  XX        XX",
"XXXXXXX  XX  XXXXX  XXXXX",
"XXXXXXX  XX    XXX  XXXXX",
"X   XXX        XXX  XXXXX",
"X   XXX  XXXXXXXXXXXXXXXX",
"X           XXXXXXXXXXXXX",
"X                  XXXXXX",
"XXXXXXXXXXXXXX     XXX  X",
"XXXXXXXXXXXXXXXX   XXX  X",
"XXX   XXXXXXXXXX        X",
"XXX                     X",
"XXX         XXXXXXXXXXXXX",
"XXXXXXXXXX  XXXXXXXXXXXXX",
"XXXXXXXXXX              X",
"XX   XXXXX              X",
"XX   XXXXXXXXXXXXXXX  XXX",
"XX      XXXXXXXXXXXX  XXX",
"XXXX         XXXXX      X",
"XXXXXXX                 X",
"XXXXXXXXXXXXXXXXXXXXXXXXX"
]

def setup_maze():
    for y in range(len(level)):
        for x in range(len(level[y])):
            character = level[y][x]
            screen_x = -288 + (x * 24)
            screen_y = 288 - (y * 24)
            if character == "X":
                pen.goto(screen_x, screen_y)
                pen.stamp()
                walls.append((screen_x, screen_y))
            if character == "P":
                player.goto(screen_x, screen_y)

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + ' > I heard %s', data.data)
    if data.data == 'Left':
        pyautogui.press('left')
    if data.data == 'Right':
        pyautogui.press('right')
    if data.data == 'Up':
        pyautogui.press('up')
    if data.data == 'Down':
        pyautogui.press('down')

def subscriber():
    rospy.init_node('subscriber', anonymous=True)
    rospy.Subscriber('chatter', String, callback)

wn = turtle.Screen()
wn.bgcolor('black')
wn.title('Maze Game')
wn.setup(700, 700)

pen = Pen()
player = Player()
walls = []

setup_maze()

wn.tracer(0)

turtle.listen()
turtle.onkey(player.go_left, 'Left')
turtle.onkey(player.go_right, 'Right')
turtle.onkey(player.go_up, 'Up')
turtle.onkey(player.go_down, 'Down')

subscriber()

while True:
    wn.update()
