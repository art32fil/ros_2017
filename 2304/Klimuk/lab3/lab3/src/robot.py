#!/usr/bin/env python
import rospy
import tf
import math
from weapon_part import Part
from geometry_msgs.msg import Twist
from visualization_msgs.msg import Marker

class Robot:
	def marker_update(self):
		marker = Marker()
		marker.header.frame_id = self.id
		marker.type = marker.CUBE
		marker.action = marker.ADD
		
		marker.scale.x = 0.5
		marker.scale.y = 0.2
		marker.scale.z = 0.2
		
		marker.color.a = 1.0
		marker.color.r = self.color[0]
		marker.color.g = self.color[1]
		marker.color.b = self.color[2]
		
		marker.pose.position.x = 0
		marker.pose.position.y = 0
		marker.pose.position.z = 0
		marker.pose.orientation.w = 1
		
		self.publisher.publish(marker)
		
		quaternion = tf.transformations.quaternion_from_euler(0, 0, self.orientation)
		br = tf.TransformBroadcaster()
		br.sendTransform((self.position[0], self.position[1], 0), quaternion, rospy.Time.now(), self.id, "map")

	def addVelocity(self, lin, ang):
		self.lin_vel = lin
		self.ang_vel = ang

	def position_update(self, delta_time):
		self.orientation += self.speed * self.ang_vel * delta_time
		self.orientation -= 2 * math.pi * math.floor((self.orientation + math.pi)/(2*math.pi))
		
		force = self.speed * self.lin_vel*delta_time
		if (force > 1):
			force = 1
			
		if (force < -1):
			force = -1 
			
		self.position[0] += math.sin(self.orientation + math.pi/2) * force
		self.position[1] -= math.cos(self.orientation + math.pi/2) * force
		
		self.ang_vel = 0
		self.lin_vel = 0
		
	def update(self, delta_time):
		if (self.isLife):
			self.position_update(delta_time)
			self.marker_update()
			
			for weapon_pt in self.weapon_parts:
				weapon_pt.update(delta_time)

	def destroy(self):
		self.isLife = False
		
	def __init__(self, id, speed, color, pos):
		self.id = id
		self.isLife = True
		self.color = color
		self.speed = speed
		self.position = pos
		self.orientation = 0.0
		self.publisher = rospy.Publisher(self.id, Marker, queue_size = 100)
		self.ang_vel = 0
		self.lin_vel = 0
		self.marker_update()
		
		self.weapon_parts = []
		i = 0
		weapon_id = self.id + "_weapon" 
		while i < 3:
			position = (0,1,0)
			parent = weapon_id
			if (i == 0):
				parent = self.id
				position = (0.1,0,0)
			else:
				parent += "_" + str(i-1)
			
			if (i == 2):
				part = Part(weapon_id + "_" + str(i), Marker.SPHERE, parent, speed, [1,0,1], position, [0.4,0.4,0.4])
				self.weapon_parts.append(part)
				self.damage_part_id = weapon_id + "_" + str(i)
			else:
				part = Part(weapon_id + "_" + str(i), Marker.CYLINDER, parent, speed, color, position, [0.1,1,0.1])
				self.weapon_parts.append(part)
			i += 1
			
		