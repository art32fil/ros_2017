#!/usr/bin/env python
import rospy
import tf
import math
from robot import Robot
from lab_msgs.msg import L3RobotMes
from lab_srvs.srv import L3RegRobot, L3RegRobotResponse
from lab_srvs.srv import L3BotGoal, L3BotGoalResponse

robots = {}
tfListener = tf.TransformListener()

def position_srv(req):
	resp = L3BotGoalResponse()
	
	if "player" in robots:
		try:
			position, quaternion = tfListener.lookupTransform("/"+req.id,"/player", rospy.Time())
			resp.player_position_x = position[0]
			resp.player_position_y = position[1]
		except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
			pass
	else:
		rospy.logerr("Player not find")
		
	if req.id in robots:
		resp.bot_orientation = float(robots[req.id].orientation)
	else:
		rospy.logerr("bot not find")
		
	return resp
	
def register_robot(req):
	if req.id in robots:
		rospy.logerr("Register Failed")
	else:
		color = [req.color_r, req.color_g, req.color_b]
		pos = [req.position_x, req.position_y]
		robots[req.id] = Robot(req.id, req.speed, color, pos)
	return L3RegRobotResponse()

def get_input(data):
	id = data.id
	vel = data.velocity
	if id in robots:
		robots[id].addVelocity(int(vel.linear.x),int(vel.angular.z))
	else:
		print "Wrong ID"
	
def server():
	rospy.init_node("server")
	tfListener = tf.TransformListener()
	last_update = rospy.Time.now().to_sec()
	
	posSrv = rospy.Service('position_service', L3BotGoal, position_srv)
	regSrv = rospy.Service('register_service', L3RegRobot, register_robot)
	rospy.Subscriber('robots_vels',L3RobotMes,get_input)
	
	R = rospy.Rate(150)
	
	rospy.sleep(5)
	while not rospy.is_shutdown():
		delta_time = rospy.Time.now().to_sec() - last_update
		
		for robot in robots:
			robots[robot].update(delta_time)
		
		for robot in robots:
			if robots[robot].isLife:
				for robot2 in robots:
					try:
						if robot != robot2:
							position, quaternion = tfListener.lookupTransform("/"+robots[robot].damage_part_id, "/"+robots[robot2].id, rospy.Time())
							distance = math.sqrt(position[0] * position[0] + position[1] * position[1])
							if distance < 0.3:
								robots[robot2].destroy()
					except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
						continue
		R.sleep()
		last_update = rospy.Time.now().to_sec()
		
		


if __name__ == '__main__':
	try:
		server()
	except rospy.ROSInterruptException:
		pass