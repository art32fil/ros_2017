#!/usr/bin/env python
from threading import Thread

import rospy
from std_msgs.msg import String


class Controller:
    def __init__(self, topic, sleep):
        self.topic = topic
        self.sleep = sleep
        self.thread = None

        self.pub = rospy.Publisher(topic, String, queue_size=10)

    def run(self):
        rospy.sleep(self.sleep)

    def start(self):
        self.thread = Thread(target=self.run)
        self.thread.start()
