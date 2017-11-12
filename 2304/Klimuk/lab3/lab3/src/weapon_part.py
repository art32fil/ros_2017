#!/usr/bin/env python
import rospy
import tf
import math
from random import randint
from geometry_msgs.msg import Twist
from visualization_msgs.msg import Marker

class Part:
	def marker_update(self):
		marker = Marker()
		marker.header.frame_id = self.id
		marker.type = self.type
		marker.action = marker.ADD
		
		marker.scale.x = self.scale[0]
		marker.scale.y = self.scale[1]
		marker.scale.z = self.scale[2]
		
		marker.color.a = 1.0
		marker.color.r = self.color[0]
		marker.color.g = self.color[1]
		marker.color.b = self.color[2]
		
		marker.pose.position.x = 0
		marker.pose.position.y = self.scale[1] * 0.5
		marker.pose.position.z = 0
		marker.pose.orientation.w = 1
		
		self.publisher.publish(marker)
		
		quaternion = tf.transformations.quaternion_from_euler(0, 0, self.orientation)
		br = tf.TransformBroadcaster()
		br.sendTransform((self.position[0], self.position[1], 0), quaternion, rospy.Time.now(), self.id, self.parent)

	def rotation_update(self, delta_time):
		self.orientation += self.speed * self.ang_vel * delta_time
		self.orientation -= 2 * math.pi * math.floor((self.orientation + math.pi)/(2*math.pi))
		
	def update(self, delta_time):
		self.rotation_update(delta_time)
		self.marker_update()

	def __init__(self, id, type, parent, speed, color, pos, scale):
		self.id = id
		self.color = color
		self.speed = speed
		self.position = pos
		self.type = type
		self.scale = scale
		self.orientation = 0.0
		self.publisher = rospy.Publisher(self.id, Marker, queue_size = 100)
		self.ang_vel = randint(1,2)
		self.parent = parent
		self.marker_update()