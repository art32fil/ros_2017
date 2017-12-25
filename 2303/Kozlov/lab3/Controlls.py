#!/usr/bin/env python
import rospy
import curses
from Getch import Getch
from std_msgs.msg import String

id = "robot0"
publisher = ""
getch = Getch()
end = False

def io():
    global end
    key = getch()
    if curses.KEY_UP:
        # Up
        publisher.publish(id + " 0 1")
    elif curses.KEY_DOWN:
        # Down
        publisher.publish(id + " 0 -1")
    elif curses.KEY_LEFT:
        # Left
        publisher.publish(id + " -1 0")
    elif curses.KEY_RIGHT:
        # Right
        publisher.publish(id + " 1 0")
    elif key == 'e' or key == 'E':
        # Exit
        publisher.publish("e")
        end = True


def input():
    rospy.init_node('input')

    global publisher
    publisher = rospy.Publisher('inputs', String, queue_size=20)

    while not (rospy.is_shutdown() or end):
        io()
        rospy.sleep(0.01)

if __name__ == '__main__':
    try:
        input()
    except rospy.ROSInterruptException: pass


