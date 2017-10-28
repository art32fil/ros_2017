#!/usr/bin/env python

import rospy
from pynput.keyboard import Key, Listener, KeyCode
from std_msgs.msg import String

class KeyPublisher:
    KEY_Q = KeyCode(char='q')
    KEY_W = KeyCode(char='w')
    KEY_A = KeyCode(char='a')
    KEY_S = KeyCode(char='s')
    KEY_D = KeyCode(char='d')
    keys = (KEY_Q, KEY_W, KEY_A, KEY_S, KEY_D, Key.esc)

    def __init__(self, topic):
        self.topic = topic
        self.pub = rospy.Publisher(topic, String, queue_size=10)
        rospy.init_node('publisher', anonymous=True)

    def start(self):
        def __on_press(key):
            if key in self.keys:
                if key == self.KEY_W:
                    self.pub.publish('Up')
                    rospy.loginfo('I send Up')
                if key == self.KEY_S:
                    self.pub.publish('Down')
                    rospy.loginfo('I send Down')
                if key == self.KEY_A:
                    self.pub.publish('Left')
                    rospy.loginfo('I send Left')
                if key == self.KEY_D:
                    self.pub.publish('Right')
                    rospy.loginfo('I send Right')

        def __on_release(key):
            if key in (Key.esc, self.KEY_Q):
                return False

        rospy.loginfo('Press w to send Up')
        rospy.loginfo('Press s to send Down')
        rospy.loginfo('Press a to send Left')
        rospy.loginfo('Press d to send Right')
        rospy.loginfo('Press esc or q to exit')

        with Listener(on_press=__on_press, on_release=__on_release) as listener:
            listener.join()

if __name__ == '__main__':
    try:
        talker = KeyPublisher('chatter')
        talker.start()
    except rospy.ROSInterruptException:
        pass
