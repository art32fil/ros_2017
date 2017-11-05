#!/usr/bin/env python

import rospy
from interactive_markers.interactive_marker_server import \
    InteractiveMarkerServer

from ai_controller import AiController
from button_controller import ButtonController
from robot import Robot
from weapon import Weapon

topic = 'robots'


def duration(sec):
    return rospy.Duration(0, sec * 10e8)


def main():
    rospy.init_node(topic)
    server = InteractiveMarkerServer(topic)
    sleep = duration(0.05)

    robot0 = Robot('Player', 0, 2, server)
    robot0.add_weapon(Weapon(robot0.int_marker))
    player = ButtonController(robot0.id, sleep)

    robot1 = Robot('Chris', 2, 0, server)
    ai1 = AiController(robot1.id, sleep)

    robot2 = Robot('Andy', -2, 0, server)
    ai2 = AiController(robot2.id, sleep)

    robot3 = Robot('Miku', 0, -2, server)
    ai3 = AiController(robot3.id, sleep)

    player.start()
    ai1.start()
    ai2.start()
    ai3.start()

    def player_alive(_):
        if not player.thread.isAlive():
            ai1.stop()
            ai2.stop()
            ai3.stop()
            rospy.signal_shutdown('Stopped by user')

    rospy.Timer(sleep, player_alive)
    rospy.spin()


if __name__ == '__main__':
    main()
