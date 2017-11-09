#!/usr/bin/env python
import rospy
import tf
import math
import random
from visualization_msgs.msg import Marker

class Hand:
    def physic(self, delta_time):
        self.rotation += random.random() * self.speed * 700 * delta_time

        if self.rotation > 360:
            self.rotation -= 360

        if self.rotation < 0:
            self.rotation += 360


    def ros(self):
        br = tf.TransformBroadcaster()
        br.sendTransform(self.position,
                         tf.transformations.quaternion_from_euler(0, 0, math.radians(self.rotation)),
                         rospy.Time.now(),
                         self.id,
                         self.parent)

        marker = Marker()
        marker.header.frame_id = self.id
        marker.type = marker.CYLINDER
        marker.action = marker.ADD
        marker.scale.x = self.length
        marker.scale.y = 0.1
        marker.scale.z = 0.1
        marker.color.a = 1.0
        marker.color.r = self.color[0]
        marker.color.g = self.color[1]
        marker.color.b = self.color[2]
        marker.pose.orientation.w = 1.0
        marker.pose.position.x = self.length / 2.0
        marker.pose.position.y = 0
        marker.pose.position.z = 0
        self.publisher.publish(marker)



    def update(self, delta_time):
        self.physic(delta_time);
        self.ros();

    def __init__(self, id, parent, position, length, color, speed):
        self.id = id
        self.parent = parent
        self.position = position
        self.color = color
        self.speed = speed
        self.length = length
        self.rotation = 0.0
        self.publisher = rospy.Publisher(self.id, Marker, queue_size=10)
        self.ros()

    def __call__(self, delta_time): return self.update(delta_time)