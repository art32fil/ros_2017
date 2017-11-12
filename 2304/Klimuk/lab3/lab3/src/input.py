#!/usr/bin/env python
import rospy
import curses 
from lab_msgs.msg import L3RobotMes
from lab_srvs.srv import L3RegRobot
from geometry_msgs.msg import Twist

id = "player"
def publisher(win):
	pub = rospy.Publisher('robots_vels', L3RobotMes, queue_size=100)
	rospy.init_node('player_input', anonymous=True)
	rospy.wait_for_service('register_service')
	
	try: 
		regSrv = rospy.ServiceProxy('register_service', L3RegRobot)
		if rospy.get_param("~color") == "red":
			color_r = 1
			color_g = 0
			color_b = 0	
		elif rospy.get_param("~color") == "green":
			color_r = 0
			color_g = 1
			color_b = 0
		elif rospy.get_param("~color") == "blue":
			color_r = 0
			color_g = 0
			color_b = 1
		else:
			color_r = 1
			color_g = 1
			color_b = 1	
		position = rospy.get_param("~position").split()
		
		regSrv(id, color_r,color_g, color_b, float(rospy.get_param("~speed")), float(position[0]), float(position[1]))
	except rospy.ServiceException, e:
		rospy.logerr("Service call failed: %s"%e)
	
	while not rospy.is_shutdown():
		c = win.getch()
		
		msg = L3RobotMes()
		msg.id = id
		
		velocity = Twist()
		velocity.linear.x = 0;
		velocity.angular.z = 0;
		
		if c == ord('w'):
			velocity.linear.x = -1;
		elif c == ord('s'):
			velocity.linear.x = 1;
			
		if c == ord('d'):
			velocity.angular.z = -1;
		elif c == ord('a'):
			velocity.angular.z = 1;
				
		msg.velocity = velocity
		pub.publish(msg)
		if c == ord('q'):
			break  # Exit the while loop
			
if __name__ == '__main__':
	try:
		curses.wrapper(publisher)
	except rospy.ROSInterruptException:
		pass