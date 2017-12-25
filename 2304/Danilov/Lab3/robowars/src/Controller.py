#!/usr/bin/env python
import rospy
from std_msgs.msg import String

class Controller:
    def __init__(self, id):
        self.id = id
        self.publisher = rospy.Publisher('inputs', String, queue_size=20)
    
    def send_command(self, command):
        self.publisher.publish(self.id + " " + command)
        
