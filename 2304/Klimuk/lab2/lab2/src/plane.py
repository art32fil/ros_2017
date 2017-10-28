#!/usr/bin/env python
import rospy
import sys
from lab_msgs.msg import L2PlaneMes
from lab_srvs.srv import *
from random import choice
from random import randint
from string import ascii_uppercase
from time import sleep

codes = []

def repair(req):
	resp = L2RepairPlaneResponse()
	for code in codes:
		if (code == req.repair_code):
			codes.remove(code)
			resp.right_code = True
			resp.message = "Thanks for help!!!"
			return resp
			
	resp.right_code = False
	resp.message = "Nooooooooo... It`s wrong code!!!"
	return resp


def publisher(difficult):
	pub = rospy.Publisher('dispatcher_plane_channel', L2PlaneMes, queue_size=20)
	srv = rospy.Service('repair_service', L2RepairPlane, repair)
	
	rospy.init_node("plane", anonymous=True)
	errorCount = 0
	wait = 0
	i = 0
	messages = ["Heeeeelp", "Fire in the hall, fire in the hall", "WE ALL DIIIIE", "So, can you help?"]
	while not rospy.is_shutdown():
		#generate random code
		newCode = ''.join(choice(ascii_uppercase) for i in range(difficult))
		codes.append(newCode)
		sleep(randint(3,8))
		
		size = 0
		for x in codes:
			size += 1
			
		if (size > 4):
			msg = L2PlaneMes()
			msg.gameOver = True
			pub.publish(msg)
			rospy.signal_shutdown("FAILED")
			
		msg = L2PlaneMes()
		msg.code = newCode
		msg.message = choice(messages)
		msg.gameOver = False
		pub.publish(msg)
			
		
			
if __name__ == '__main__':
	try:
		if len(sys.argv) == 2:
			publisher(int(sys.argv[1]))
		else:
			publisher(5)
	except rospy.ROSInterruptException:
		pass
	
