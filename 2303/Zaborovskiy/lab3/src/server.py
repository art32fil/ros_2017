#!/usr/bin/env python
import rospy
import math
from robot import Robot
from std_msgs.msg import String

robots = {}
end = False


def new_input(data):
    if data.data != "e":
        arr = data.data.split()
        id = arr[0]
        if id in robots:
            x = arr[1]
            y = arr[2]
            robots[id].move(int(x), int(y))
    else:
        print "Shotdown message"
        rospy.signal_shutdown("FAILED")

def server():
    rospy.init_node('server')
    last_update = rospy.Time.now().to_sec()

    robots["robot0"] = Robot("robot0", [0, 1, 0], 100, [4, 0])
    robots["robot1"] = Robot("robot1", [1, 0, 0], 100, [-4, 0])
    robots["robot2"] = Robot("robot2", [1, 0, 0], 100, [0, 4])
    robots["robot3"] = Robot("robot3", [1, 0, 0], 100, [0, -4])

    rospy.Subscriber('inputs', String, new_input)

    while not (rospy.is_shutdown() or end):
        delta_time = rospy.Time.now().to_sec() - last_update
        #print (delta_time)

        #player(delta_time)
        #bot(delta_time)

        for k, v in robots.items():
            v(delta_time)

        rospy.sleep(0.01)
        last_update = rospy.Time.now().to_sec()

if __name__ == '__main__':
    try:
        server()
    except rospy.ROSInterruptException: pass


