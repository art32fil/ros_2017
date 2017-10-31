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
			pub.publish(ord('u'))
		if c == ord('a'):
			pub.publish(ord('l'))
		if c == ord('s'):
			pub.publish(ord('d'))
		if c == ord('d'):
			pub.publish(ord('r'))
		if c == ord('z'):
			pub.publish(ord('z'))
		if c == ord('x'):
			pub.publish(ord('x'))	
		if c == ord('q'):
			break  # Exit the while loop
			
if __name__ == '__main__':
	try:
		curses.wrapper(publisher)
	except rospy.ROSInterruptException:
		pass
	
