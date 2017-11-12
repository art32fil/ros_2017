#!/usr/bin/env python
import rospy
from getch import _Getch
from std_msgs.msg import String

id = "robot0"
publisher = ""
getch = _Getch()
end = False

def io():
    global end
    key = getch()
    if key == 'a' or key == 'A':
        # Left
        publisher.publish(id + " -1 0")
    elif key == 'd' or key == 'D':
        # Right
        publisher.publish(id + " 1 0")
    elif key == 'w' or key == 'W':
        # Up
        publisher.publish(id + " 0 1")
    elif key == 's' or key == 'S':
        # Down
        publisher.publish(id + " 0 -1")
    elif key == 'e' or key == 'E':
        # Exit
        publisher.publish("e")
        end = True
    elif key == 'c' or key == 'C':
        # Cheatcode
        publisher.publish(id + " iddqd")

def input():
    rospy.init_node('input')

    global publisher
    publisher = rospy.Publisher('inputs', String, queue_size=20)

    rospy.sleep(5)
    print ("Touch W A S D to move")
    print ("Touch C to back again")
    print ("Touch E to exit")
    
    while not (rospy.is_shutdown() or end):
        io()
        rospy.sleep(0.01)

if __name__ == '__main__':
    try:
        input()
    except rospy.ROSInterruptException: pass


