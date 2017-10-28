#!/usr/bin/env python

import rospy
from std_msgs.msg import String
import random

from pynput.keyboard import Key, Listener, KeyCode

def generate_key():
    key = random.randint(1,6)
    if key == 1: return "up"
    if key == 2: return "down"
    if key == 3: return "left"
    if key == 4: return "right"
    if key == 5: return "print"
topik = "lab_1"

_Q_ = KeyCode(char='q')
_W_ = KeyCode(char='w')
_A_ = KeyCode(char='a')
_S_ = KeyCode(char='s')
_D_ = KeyCode(char='d')
_P_ = KeyCode(char='p')

keys = (_Q_, _W_, _A_, _S_, _D_, _P_, Key.esc)
pub = rospy.Publisher(topik, String, queue_size=1000)
def on_press(key):
    if key in keys:
        rospy.loginfo('got key %s', key)
        if key == _W_:
            pub.publish('up')
        if key == _S_:
            pub.publish('down')
        if key == _A_:
            pub.publish('left')
        if key == _D_:
            pub.publish('right')
        if key == _P_:
            pub.publish('print')

def on_release(key):
    if key in (Key.esc, _Q_):
        return False

def talker():
    rospy.init_node('talker', anonymous=True)
    #rate = rospy.Rate(10) # 10hz
    
    with Listener(on_press=on_press, on_release=on_release
) as listener:
        listener.join()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
