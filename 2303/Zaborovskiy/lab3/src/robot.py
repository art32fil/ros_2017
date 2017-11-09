#!/usr/bin/env python
import rospy
import tf
import math
from hand import Hand
from visualization_msgs.msg import Marker

HANDS_COUNT = 3
HAND_LENGTH = 1

class Robot:
    def move(self, x, y):
        self.acceleration[0] = x
        self.acceleration[1] = y

    def physic(self, delta_time):
        self.rotation -= self.acceleration[0] * self.speed * delta_time

        if self.rotation > 360:
            self.rotation -= 360

        if self.rotation < 0:
            self.rotation += 360

        force = self.acceleration[1] * self.speed * delta_time

        self.position[0] += math.cos(self.rotation) * force
        self.position[1] += math.sin(self.rotation) * force

        self.acceleration[0] = 0
        self.acceleration[1] = 0

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

        marker.pose.orientation.w = 1

        marker.pose.position.x = 0
        marker.pose.position.y = 0
        marker.pose.position.z = 0
        self.publisher.publish(marker)

        quaternion = tf.transformations.quaternion_from_euler(0, 0, self.rotation)
        br = tf.TransformBroadcaster()
        br.sendTransform((self.position[0], self.position[1], 0),
                         quaternion,
                         rospy.Time.now(),
                         self.id,
                         "map")


    def update(self, delta_time):
        if self.isLife:
            self.physic(delta_time);
            self.ros();

            for v in self.hands:
                v(delta_time)

    def destroy(self):
        self.isLife = False

    def __init__(self, id, color, speed, pos):
        self.id = id
        self.isLife = True
        self.color = color
        self.speed = speed
        self.acceleration = [0, 0]
        self.position = pos
        self.rotation = 0.0
        self.publisher = rospy.Publisher(self.id, Marker, queue_size=10)
        self.ros()

        self.hands = []
        i = 0
        while i < HANDS_COUNT + 1:
            me = self.id + "_hand" + str(i)

            position = (HAND_LENGTH, 0, 0)
            parent = self.id + "_hand" + str(i - 1)

            if i == 0 :
                parent = self.id
                position = (0.4, 0, 0)

            hand = Hand(me, parent, position, HAND_LENGTH, [1,1,1], 10 * i * i * i)
            self.hands.append(hand)

            i += 1

    def __call__(self, delta_time): return self.update(delta_time)