#!/usr/bin/env python
import rospy
import curses 
from std_msgs.msg import Char

def publisher(win):
	pub = rospy.Publisher('movement_direction', Char, queue_size=10)
	rospy.init_node('publisher', anonymous=True)
	
	while not rospy.is_shutdown():
		c = win.getch()

		if c == ord('w'):
			pub.publish(ord('w'))
		if c == ord('a'):
			pub.publish(ord('a'))
		if c == ord('s'):
			pub.publish(ord('s'))
		if c == ord('d'):
			pub.publish(ord('d'))
		if c == ord('q'):
			pub.publish(ord('q'))
			break  
			
if __name__ == '__main__':
	try:
		curses.wrapper(publisher)
	except rospy.ROSInterruptException:
		pass
	