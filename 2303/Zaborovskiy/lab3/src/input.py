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
    if key == 'a' or key == 'A' or key == 'Ф' or key == 'ф':
        # Left
        publisher.publish(id + " -1 0")
    elif key == 'd' or key == 'D' or key == 'В' or key == 'в':
        # Right
        publisher.publish(id + " 1 0")
    elif key == 'w' or key == 'W' or key == 'Ц' or key == 'ц':
        # Up
        publisher.publish(id + " 0 1")
    elif key == 's' or key == 'S' or key == 'Ы' or key == 'ы':
        # Down
        publisher.publish(id + " 0 -1")
    elif key == 'e' or key == 'E' or key == 'У' or key == 'у':
        # Exit
        publisher.publish("e")
        end = True
    elif key == 'c' or key == 'C' or key == 'С' or key == 'с':
        # Cheatcode
        publisher.publish(id + " iddqd")

def input():
    rospy.init_node('input')

    global publisher
    publisher = rospy.Publisher('inputs', String, queue_size=20)
    
    global id
    id = rospy.get_param('~id')
    
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


