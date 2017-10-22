#!/usr/bin/env python

import sys
import rospy
from std_msgs.msg import String
from ussr.srv import *
import sys, os

############
# KEYBOARD #
############
def wait_key():
    ''' Wait for a key press on the console and return it. '''
    result = None
    if os.name == 'nt':
        import msvcrt
        result = msvcrt.getch()
    else:
        import termios
        fd = sys.stdin.fileno()

        oldterm = termios.tcgetattr(fd)
        newattr = termios.tcgetattr(fd)
        newattr[3] = newattr[3] & ~termios.ICANON & ~termios.ECHO
        termios.tcsetattr(fd, termios.TCSANOW, newattr)

        try:
            result = sys.stdin.read(1)
        except IOError:
            pass
        finally:
            termios.tcsetattr(fd, termios.TCSAFLUSH, oldterm)

    return result

###############
# TOPIC - GET #
###############
globalCountry = ""
def callback(data):
    data = str(data.data)# Example: "Ukraine"
    rospy.loginfo('%s has separated from the Soviet Union', data)
    globalCountry = data

#################
# MAIN FUNCTION #
#################
def publisher():    
    rospy.init_node('subsriber', anonymous=True)# TOPIC - INIT
    rospy.Subscriber('newspaper', String, callback)# TOPIC - CONNECT

    print("Let's look at the political map of 1991:")

    rate = rospy.Rate(0.5)#
    while not rospy.is_shutdown():
        butt = wait_key()
        if butt == "s":# SERVICE - START
            rospy.wait_for_service('save_country')# SERVICE - CONNECT
            try:
                save_country = rospy.ServiceProxy('save_country', AddTwoInts)# SEND BY /srv/AddTwoInts.srv mask
                resp1 = save_country("Last")# SERVICE - REQUEST
                rospy.loginfo("%s is saved", resp1.response)# SERVICE - RESPONSE
            except rospy.ServiceException, e:
                print "Service call failed: %s"%e# SERVICE - FINISH
        else:
            rospy.loginfo("Type 's' to save the country")
        #rate.sleep()

#################
# PROGRAM START #
#################
if __name__ == '__main__':
    try:
        publisher()
    except rospy.ROSInterruptException:
        pass
