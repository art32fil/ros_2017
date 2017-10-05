#!/usr/bin/env python
import rospy
from lab_msgs.msg import L2RepairMes

def repairer():
	pub = rospy.Publisher('dispatcher_repairer_channel', L2RepairMes, queue_size=20)
	rospy.init_node("plane", anonymous=True)
	
	while not rospy.is_shutdown():
		code = raw_input("Enter next help code: ")
		pub.publish(L2RepairMes(code))
			
if __name__ == '__main__':
	try:
		repairer()
	except rospy.ROSInterruptException:
		pass
	
