#!/usr/bin/env python

import rospy
import os
from std_msgs.msg import Char

class Labyrinth:
	def __init__(self, map_file):
		self.map = []
		with open(map_file) as file:
			for line in file:
				char_line = []
				for char in line:
					char_line.append(char)
				self.map.append(char_line)
					
	def isFree(self, x, y):
		return self.map[x][y] == '*'
	
	def isFinish(self, x, y):
		return self.map[x][y] == '?'
		
	def print_map(self):
		map_str = "\n\n"
		for line in self.map:
			for char in line:
				map_str += char
		return map_str
		
class Robot:
	def Start(self, level):
		i = 0
		for line in level.map:
			j = 0
			for char in line:
				if char == '%':
					self.x = i
					self.y = j 
					break
				j += 1
			i += 1

	def change(self, map, dx, dy):
		map[self.x][self.y] = '*'
		self.x += dx
		self.y += dy
		map[self.x][self.y] = '%'	

	def move(self, direction, level):
		dx = 0
		dy = 0
		if direction == 119: #w
			dx -= 1		
		if direction == 115: #s
			dx += 1
		if direction == 97: #a
			dy -= 1
		if direction == 100: #d
			dy += 1
		
		if level.isFree(self.x + dx, self.y + dy):	
			self.change(level.map, dx,dy)
		elif level.isFinish(self.x + dx, self.y + dy):
			self.change(level.map, dx,dy)
			rospy.loginfo("\n\n!!!Finish!!!\n")	
			rospy.signal_shutdown("Fin")
			


def subscriber():
	res_path = os.path.dirname(os.path.abspath(__file__))
	level = Labyrinth(res_path + "/labyr.txt")
	robot = Robot()
	robot.Start(level)
	
	def sub_callback(data):
		if data.data == 113:
			rospy.loginfo("Program ended")
			rospy.signal_shutdown("End")
		robot.move(data.data, level)
		rospy.loginfo(level.print_map())
		
		
	sub = rospy.Subscriber('movement_direction', Char, sub_callback)
	rospy.init_node('subscriber', anonymous=True)
	rospy.spin()
		
if __name__ == '__main__':
	subscriber()
	