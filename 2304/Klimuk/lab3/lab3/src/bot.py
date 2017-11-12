#!/usr/bin/env python
import rospy
from lab_msgs.msg import L3RobotMes
from lab_srvs.srv import L3RegRobot, L3BotGoal
from geometry_msgs.msg import Twist
from random import randint
from math import pow,atan2,sqrt, pi

def bot():
	pub = rospy.Publisher('robots_vels', L3RobotMes, queue_size=100)
	rospy.init_node('bot_input', anonymous=True)
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
		
		regSrv(rospy.get_param("~id"), color_r, color_g, color_b, float(rospy.get_param("~speed")), float(position[0]), float(position[1]))
	except rospy.ServiceException, e:
		rospy.logerr("Service call failed: %s"%e)
	
	rospy.wait_for_service('position_service')
	PosSrv = rospy.ServiceProxy('position_service', L3BotGoal)
	while not rospy.is_shutdown():
		response = PosSrv(rospy.get_param("~id"))
		
		x_dif = response.player_position_x
		y_dif = response.player_position_y
		
		distance = sqrt(pow((x_dif), 2) + pow((y_dif), 2))
		if (distance >= 0.2):
			msg = L3RobotMes()
			msg.id = rospy.get_param("~id")
			velocity = Twist()
			
			velocity.linear.x = distance
			velocity.angular.z = 4 *(atan2(y_dif, x_dif) - response.bot_orientation)
			
			msg.velocity = velocity
			pub.publish(msg)
			
if __name__ == '__main__':
	try:
		bot()
	except rospy.ROSInterruptException:
		pass