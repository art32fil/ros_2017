#!/usr/bin/env python
import rospy
import sys
from lab2.msg import *
from lab2.srv import *
from random import randint
import time

level = 10
answer = 42
need_question = True
last_question = 0
right_answer = False
numbers = []

def test_answer(req):
    global need_question
    global right_answer
    global last_question
    global level

    num = req.num
    num_sum = sum(numbers)
    answ = "CORRECT"
    if num == num_sum :
        need_question = True
        if level > 4:
            level-=1
    elif num == answer:
        right_answer = True
        answ = "'The Ultimate Question of Life, the Universe, and Everything' - founded!!!"
    else:
        last_question -= 5
        answ = "INCORRECT"
    return answ

def create_question():
    global need_question
    global last_question
    global right_answer
    global numbers       
    
    need_question = False
    numbers[:] = []
    i = answer
    while i > 0:
        r = randint(1, level)
        if r > i:
            r = i
        i -= r
        numbers.append(r)

    p = numbers[randint(0, len(numbers) - 1)]
    numbers.remove(p)
    need_question = False
    last_question = int(time.time()) + answer

def main_thread():
    print "Deep Thought started"

    pub = rospy.Publisher('deep_thought_process', NotString, queue_size=20)
    qpub = rospy.Publisher('deep_thought_question', NotString, queue_size=20)

    rospy.sleep(1.0)
    pub.publish("Welcome to Deep Thought!")
    rospy.sleep(1.0)
    pub.publish("Please, help me will respond to 'The Ultimate Question of Life, the Universe, and Everything'.")
    rospy.sleep(4.0)
    pub.publish("I find it and then I lose it parts all the time!!!")
    rospy.sleep(4.0)
    pub.publish("Just send me the sum of the numbers. Verifying by hash code will help me find the whole.")
    rospy.sleep(5.0)

    while not rospy.is_shutdown() and not right_answer:
        if need_question:
            create_question()
            q = ', '.join(str(n) for n in numbers)
            qpub.publish(q)
        elif last_question < int(time.time()):
            pub.publish("BSOD")
            rospy.signal_shutdown("FAILED")
        print (last_question - int(time.time())), "s"
        rospy.sleep(1.0)
    if not right_answer:
        print "CHhhshdhhfhsdfh"
    rospy.signal_shutdown("FAILED")

def data_bus(data):
    main_thread()


def deep_thought():
    rospy.init_node('deep_thought', anonymous=True)
    rospy.Service('deep_thought_service', Calculator, test_answer)
    rospy.Subscriber('deep_thought_databus', NotString, data_bus)
    rospy.spin()

        

if __name__ == "__main__":
    try:
        deep_thought()
    except rospy, e:
        rospy.logerr("DT call failed: %s" % e)