#!/usr/bin/env python
import rospy
import tf
import math
from Robot import Robot
from std_msgs.msg import String

class Game:
    
    def __init__(self):
        self.robots = {}
        self.shutdown = False
        self.listener = tf.TransformListener()
        rospy.Subscriber('inputs', String, self.__on_command__)

    def start(self):
        last_update = rospy.Time.now().to_sec()

        self.robots["robot0"] = Robot("robot0", [0, 1, 0], 4000, [4, 0])
        self.robots["robot1"] = Robot("robot1", [0, 0, 1], 4000, [-4, 0])
        self.robots["robot2"] = Robot("robot2", [0, 0, 1], 4000, [0, 4])
        self.robots["robot3"] = Robot("robot3", [0, 0, 1], 4000, [0, -4])

        R = rospy.Rate(150)

        rospy.sleep(5)
        print ("HOLD WASD to move, E to exit")

        while not (rospy.is_shutdown() or self.shutdown):
            delta_time = rospy.Time.now().to_sec() - last_update

            for k, robot1 in self.robots.items():
                if robot1.isLife:
                    robot1(delta_time)
                    for k2, robot2 in self.robots.items():
                            try:
                                if k != k2:
                                    position, quaternion = self.listener.lookupTransform("/"+robot1.damage.id, "/"+robot2.id, rospy.Time())
                                    distance = math.sqrt(position[0] * position[0] + position[1] * position[1])
                                    if distance < robot1.damage.length[0] / 2.0:
                                        robot2.destroy()
                            except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
                                continue
            R.sleep()
            last_update = rospy.Time.now().to_sec()

    

    def __on_command__(self, data):
        if data.data != "e":
            arr = data.data.split()
            id = arr[0]
            if id in self.robots:
                x = arr[1]
                y = arr[2]
                self.robots[id].move(int(x), int(y))
        else:
            self.shutdown = True
            print "Shutdown message"
            rospy.signal_shutdown("FAILED")
       
if __name__ == '__main__':
    try:
        rospy.init_node('server')
        game = Game()
        game.start()
    except rospy.ROSInterruptException:
        pass


