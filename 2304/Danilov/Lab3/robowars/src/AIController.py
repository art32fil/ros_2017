#!/usr/bin/env python
import rospy
import math
from Controller import Controller
from std_msgs.msg import String
from random import randint

class AIController(Controller):

    def __init__(self, id):
        Controller.__init__(self, id)

    def start(self):
        rospy.sleep(10)
        while not (rospy.is_shutdown()):
            if randint(-4, 4) > 2:
                self.send_command("0 1")
            else:
                self.send_command("1 0")
            rospy.sleep(0.01)


if __name__ == '__main__':
    try:
        rospy.init_node('stupidAI')
        id = rospy.get_param('~id')
        controller = AIController(id)
        controller.start()
    except rospy.ROSInterruptException: 
        pass
