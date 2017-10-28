#!/usr/bin/env python

import rospy
import rospkg
from std_msgs.msg import Char

class Labyrinth:
	def __init__(self, map_file):
		self.map = []
		self.index = 0
		with open(map_file) as file:
			for line in file:
				char_line = []
				for char in line:
					char_line.append(char)
				self.map.append(char_line)
					
	def isFree(self, x, y):
		return self.map[x][y] == '*'
	
	def isFinish(self, x, y):
		return self.map[x][y] == '#'
		
	def print_map(self):
		map_str = "\n"
		for line in self.map:
			for char in line:
				map_str += char
		return map_str
	
	def changeMap(self, map_file, index):
		self.map = []
		self.index = index % 3
		path = map_file + "labyrinth_data_" + str(self.index) + ".txt"
		with open(path) as file:
			for line in file:
				char_line = []
				for char in line:
					char_line.append(char)
				self.map.append(char_line)
		
class Robot:
	def __init__(self):
		self.x = -1
		self.y = -1 
	
	def findStartPosition(self, level):
		i = 0
		for line in level.map:
			j = 0
			for char in line:
				if char == '@':
					self.x = i
					self.y = j 
					break
				j += 1
			i += 1
		
	def move(self, direction, level):
		dx = 0
		dy = 0
		#inverted vert\hor
		if direction == 117: #w
			dx -= 1		
		if direction == 100: #s
			dx += 1
		if direction == 108: #a
			dy -= 1
		if direction == 114: #d
			dy += 1
		
		if level.isFree(self.x + dx, self.y + dy):	
			level.map[self.x][self.y] = '*'
			self.x += dx
			self.y += dy
			level.map[self.x][self.y] = '@'
		elif level.isFinish(self.x + dx, self.y + dy):
			rospy.loginfo("Finish!!!")	
			
def subscriber():
	rospack = rospkg.RosPack()
	res_path = rospack.get_path('lab1') + '/res/'
	level = Labyrinth(res_path + "labyrinth_data_0.txt")
	robot = Robot()
	robot.findStartPosition(level)
	
	def sub_callback(data):
		if data.data == 122: #z
			level.changeMap(res_path, level.index - 1)
			robot.findStartPosition(level)
		elif data.data == 120: #x
			level.changeMap(res_path, level.index + 1)
			robot.findStartPosition(level)
		
		robot.move(data.data, level)
		rospy.loginfo(level.print_map())
		
	sub = rospy.Subscriber('movement_direction', Char, sub_callback)
	rospy.init_node('subscriber', anonymous=True)
	rospy.spin()
		
if __name__ == '__main__':
	subscriber()
	