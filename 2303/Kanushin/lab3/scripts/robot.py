from random import randint

import rospy
from geometry_msgs.msg import Point
from std_msgs.msg import String
from visualization_msgs.msg import *


class Robot:
    step = 0.05

    def __init__(self, name, x, y, server):
        self.name = name
        self.server = server

        self.subscribe(self.id)

        self.int_marker = InteractiveMarker()
        self.int_marker.header.frame_id = "base_link"
        self.int_marker.pose.position = Robot.to_point(x, y)
        self.int_marker.scale = 1
        self.int_marker.name = self.id
        self.int_marker.description = name

        self.body = Marker()
        self.body.type = Marker.CUBE
        self.body.scale.x = self.int_marker.scale * 0.45
        self.body.scale.y = self.int_marker.scale * 0.45
        self.body.scale.z = self.int_marker.scale * 0.45
        self.body.color.r = 1.0 / float(randint(1, 5))
        self.body.color.g = 1.0 / float(randint(1, 5))
        self.body.color.b = 1.0 / float(randint(1, 5))
        self.body.color.a = 1.0


        self.control = InteractiveMarkerControl()
        self.control.markers.append(self.body)
        self.control.markers.append(self.weapon)
        self.int_marker.controls.append(self.control)

        server.insert(self.int_marker)
        server.applyChanges()

    def add_weapon(self, weapon):
        pass
        # self.int_marker.controls.append(weapon.control)
        # self.update()

    @property
    def id(self):
        return self.name.replace(' ', '_').lower()

    @staticmethod
    def to_point(x, y):
        return Point(x, y, 0)

    @staticmethod
    def from_point(point):
        return point.x, point.y

    def move_up(self):
        self.move(self.step, 0)

    def move_down(self):
        self.move(-1 * self.step, 0)

    def move_left(self):
        self.move(0, self.step)

    def move_right(self):
        self.move(0, -1 * self.step)

    def stand_still(self):
        pass

    def move(self, dx, dy):
        x, y = self.from_point(self.int_marker.pose.position)
        self.int_marker.pose.position = self.to_point(x + dx, y + dy)
        self.update()

    def update(self):
        self.server.erase(self.int_marker)
        self.server.insert(self.int_marker)
        self.server.applyChanges()

    def subscribe(self, topic):
        def callback(data):
            if data.data == 'up':
                self.move_up()

            if data.data == 'left':
                self.move_left()

            if data.data == 'right':
                self.move_right()

            if data.data == 'down':
                self.move_down()

        rospy.Subscriber(topic, String, callback)
