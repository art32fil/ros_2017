#!/usr/bin/env python
import rospy
from visualization_msgs.msg import Marker

class Robot:
    def physic(self):
        self.position[0] += self.speed[0]
        self.position[1] += self.speed[1]

    def ros(self):
        marker = Marker()
        marker.header.frame_id = "/neck"
        marker.type = marker.SPHERE
        marker.action = marker.ADD
        marker.scale.x = 0.2
        marker.scale.y = 0.2
        marker.scale.z = 0.2
        marker.color.a = 1.0
        marker.color.r = self.color[0]
        marker.color.g = self.color[1]
        marker.color.b = self.color[2]
        marker.pose.orientation.w = 1.0
        marker.pose.position.x = self.position[0];
        marker.pose.position.y = self.position[1];
        marker.pose.position.z = 0
        self.publisher.publish(marker)

    def update(self):
        self.physic();
        self.ros();

    def __init__(self, id, color):
        self.id = id
        self.color = color
        self.speed = [0, 0]
        self.position = [0, 0]
        self.publisher = rospy.Publisher(self.id, Marker, queue_size=10)
        self.ros()

    def __call__(self): return self.update()