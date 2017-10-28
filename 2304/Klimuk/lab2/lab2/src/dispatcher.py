#!/usr/bin/env python

import rospy
from lab_msgs.msg import *
from lab_srvs.srv import *

def repair_talk(data):
	try:
		repairSrv = rospy.ServiceProxy('repair_service', L2RepairPlane)
		result = repairSrv(data.code)
		rospy.loginfo(result.message)
		if (result.right_code == False):
			rospy.loginfo("[Dispatcher] What are you doing?! Write correct code to them !!!")
	except rospy.ServiceException, e:
		rospy.logerr("Service call failed: %s"%e)
	
def plane_talk(data):	
	if(data.gameOver == False):
		rospy.loginfo("\n[Dispatcher] We received new message: " + data.message + "\n[Dispatcher] Faster, write this code: " + data.code) 
	else:
		rospy.loginfo("\n[Dispatcher] Sorry, but plane was destroyed...")
		rospy.loginfo("\n\n\nYOU FAILED\n\n\n")
	
def dispatcher():
	rospy.wait_for_service('repair_service')
	planeSub = rospy.Subscriber('dispatcher_plane_channel', L2PlaneMes, plane_talk)
	repairSub = rospy.Subscriber('dispatcher_repairer_channel', L2RepairMes, repair_talk)
	
	rospy.init_node('dispatcher', anonymous=True)
	rospy.spin()
		
if __name__ == '__main__':
	dispatcher()
	