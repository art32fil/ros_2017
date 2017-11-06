#!/usr/bin/env python
import rospy
from visualization_msgs.msg import Marker

class Robot:
    def move(self, x, y):
        self.acceleration[0] = x
        self.acceleration[1] = y

    def physic(self, delta_time):
        self.position[0] += self.acceleration[0] * self.speed * delta_time
        self.position[1] += self.acceleration[1] * self.speed * delta_time

    def ros(self):
        marker = Marker()
        marker.header.frame_id = self.id
        marker.type = marker.CYLINDER
        marker.action = marker.ADD
        marker.scale.x = 0.5
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

    def update(self, delta_time):
        self.physic(delta_time);
        self.ros();

    def __init__(self, id, color, speed, pos):
        self.id = id
        self.color = color
        self.speed = speed
        self.acceleration = [0, 0]
        self.position = pos
        self.publisher = rospy.Publisher(self.id, Marker, queue_size=10)
        self.ros()

    def __call__(self, delta_time): return self.update(delta_time)