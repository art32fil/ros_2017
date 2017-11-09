#!/usr/bin/env python
import rospy
import tf
import math
from robot import Robot
from std_msgs.msg import String

robots = {}
end = False
listener = ""

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
    listener = tf.TransformListener()
    last_update = rospy.Time.now().to_sec()

    robots["robot0"] = Robot("robot0", [0, 1, 0], 400, [4, 0])
    robots["robot1"] = Robot("robot1", [1, 0, 0], 400, [-4, 0])
    robots["robot2"] = Robot("robot2", [1, 0, 0], 400, [0, 4])
    robots["robot3"] = Robot("robot3", [1, 0, 0], 400, [0, -4])

    rospy.Subscriber('inputs', String, new_input)

    R = rospy.Rate(150)
    while not (rospy.is_shutdown() or end):
        delta_time = rospy.Time.now().to_sec() - last_update

        for k, robot1 in robots.items():
            if robot1.isLife:
                robot1(delta_time)
                for k2, robot2 in robots.items():
                        try:
                            if k != k2:
                                position, quaternion = listener.lookupTransform("/"+robot1.id, "/"+robot2.id, rospy.Time())
                                distance = math.sqrt(position[0] * position[0] + position[1] * position[1])
                                if distance < 0.4:
                                    robot2.destroy()
                                print(robot1.id, " ", robot2.id, " ",distance)
                        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
                            continue
        R.sleep()
        last_update = rospy.Time.now().to_sec()

if __name__ == '__main__':
    try:
        server()
    except rospy.ROSInterruptException: pass


