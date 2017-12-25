#!/usr/bin/env python
import rospy
from Controller import Controller
from std_msgs.msg import String
import sys, tty, termios

class PlayerController(Controller):

    def __init__(self, id):
        Controller.__init__(self, id)
        self.shutdown = False

    def start(self):
        while not (rospy.is_shutdown() or self.shutdown):
            self.__io__()
            rospy.sleep(0.01)

    def __io__(self):
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        key = ch
        if key == 'a' or key == 'A':
            # Left
            self.send_command("-1 0")
        elif key == 'd' or key == 'D':
            # Right
            self.send_command("1 0")
        elif key == 'w' or key == 'W':
            # Up
            self.send_command("0 1")
        elif key == 's' or key == 'S':
            # Down
            self.send_command("0 -1")
        elif key == 'e' or key == 'E':
            # Exit
            self.publisher.publish("e")
            self.shutdown = True

if __name__ == '__main__':
    try:
        rospy.init_node('input')
        controller = PlayerController("robot0")
        controller.start()
    except rospy.ROSInterruptException: pass


