#!/usr/bin/env python
import rospy
import math
from robot import Robot
from getch import _Getch

getch = _Getch()
end = False

def input(player):
    global end
    key = getch()
    if key == 'a' or key == 'A':
        # Left
        player.speed = [-1,0]
    elif key == 'd' or key == 'D':
        # Right
        player.speed = [1, 0]
    elif key == 'w' or key == 'W':
        # Up
        player.speed = [0, 1]
    elif key == 's' or key == 'S':
        # Down
        player.speed = [0, -1]
    elif key == 'e' or key == 'E':
        # Exit
        end = True


def server():
    rospy.init_node('server')

    player = Robot("player1", [0, 1, 0])
    bot = Robot("player2", [1, 0, 0])

    while not (rospy.is_shutdown() or end):
        input(player)
        player()
        bot()
        rospy.sleep(0.01)


if __name__ == '__main__':
    try:
        server()
    except rospy.ROSInterruptException: pass


