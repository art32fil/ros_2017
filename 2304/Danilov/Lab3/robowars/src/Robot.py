#!/usr/bin/env python
import rospy
import tf
import math
from Weapon import Weapon
from visualization_msgs.msg import Marker

HANDS_COUNT = 5
HAND_LENGTH = [1,0.3,0.2,0.1,0.1]

class Robot:
    
    def __init__(self, id, color, speed, pos):
        self.id = id
        self.alive = True
        self.color = color
        self.speed = speed
        self.acceleration = [0, 0]
        self.position = pos
        self.rotation = 0.0
        self.publisher = rospy.Publisher(self.id, Marker, queue_size=10)
        self.render()

        self.weapons = []
        i = 0
        while i < HANDS_COUNT + 1:
            me = self.id + "_weapon" + str(i)

            position = (HAND_LENGTH[0], 0, 0)
            parent = self.id + "_weapon" + str(i - 1)

            if i == 0 :
                parent = self.id
                position = (0.4, 0, 0)

            weapon = Weapon(me, Marker.CYLINDER, parent, position, HAND_LENGTH, [1,1,1], 120)
            self.weapons.append(weapon)
            i += 1

        position = (1, 0, 0)
        parent = self.id + "_weapon" + str(i - 1)
        weapon = Weapon(self.id + "_damage", Marker.SPHERE, parent, position, [0.5,0.5,0.5], [1, 0, 0], 0)
        self.weapons.append(weapon)
        self.damage = weapon

    def move(self, x, y):
        self.acceleration[0] = x
        self.acceleration[1] = y

    def do_physics(self, delta_time):
        self.rotation -= self.acceleration[0] * self.speed * delta_time

        if self.rotation > 360:
            self.rotation %= 360

        if self.rotation < 0:
            self.rotation %= 360

        force = self.acceleration[1] * self.speed * delta_time

        self.position[0] += math.cos(self.rotation) * force
        self.position[1] += math.sin(self.rotation) * force

        self.acceleration[0] = 0
        self.acceleration[1] = 0

    def render(self):
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
        if self.alive:
            self.do_physics(delta_time);
            self.render();

            for v in self.weapons:
                v(delta_time)

    def destroy(self):
        self.alive = False
        marker = Marker()
        marker.header.frame_id = self.id
        marker.action = marker.DELETE
        self.publisher.publish(marker)
        for weapon in self.weapons:
            weapon.destroy()

    def __call__(self, delta_time): return self.update(delta_time)
