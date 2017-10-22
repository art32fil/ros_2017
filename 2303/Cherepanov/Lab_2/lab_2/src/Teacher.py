#!/usr/bin/env python
import rospy
import sys
from lab_2.msg import *
from lab_2.srv import *
from random import randint
from time import sleep

not_calculated = []
operations = ["+", "-", "*"]

def teacher():
    pub = rospy.Publisher('exam', Message, queue_size=20)
    srv = rospy.Service('exam_service', Exam, check)

    rospy.init_node("Maria_Andreevna", anonymous=True)

    while not rospy.is_shutdown():
        msg = Message()
        operation = operations[randint(0, len(operations) - 1)]
        number1 = randint(0, 10)
        number2 = randint(0, 10)
        if operation == "+":
            result = number1 + number2
        elif operation == "-":
            result = number1 - number2
        elif operation == "*":
            result = number1 * number2

        not_calculated.append(str(result))
        operation = str(number1) + " " + operation + " " + str(number2)
        size = len(not_calculated)

        if size > 5:
            msg = Message()
            msg.message = "Failed!"
            pub.publish(msg)
            rospy.signal_shutdown("Failed!")

        if size > 3:
            operation = "Hurry, time is ticking! Calculate this: " + operation
        else:
            operation = "Calculate this: " + operation

        msg.message = operation
        pub.publish(msg)
        sleep(randint(5, 15))

def check(request):
    response = ExamResponse()
    for number in not_calculated:
        if (number == request.answer):
            not_calculated.remove(number)
            response.result = "Correct!"
            return response
    response.result = "Wrong!"
    return response

if __name__ == '__main__':
    try:
        teacher()
    except rospy.ROSInterruptException:
        pass
