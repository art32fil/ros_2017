#include <iostream>
#include <stdio.h>

#include "../include/bot.cpp"
#include "../include/getch.cpp"
#include "ros/ros.h"
#include "std_msgs/Byte.h"

int main(int argc, char** argv)
{
  ros::init(argc, argv, "mazebot_ctrl_node");
  ros::NodeHandle n;
  ros::Publisher publisher = n.advertise<std_msgs::Byte>("mazebot_cmds_topic", 1000);
  ros::Rate loop_rate(10);
  ROS_INFO("Use WASD to control the bot");

  int key;

  while (ros::ok() && ((key = getch())))
    {
      std_msgs::Byte msg;
      switch (key)
        {
        case 'a':
          msg.data = (char)dirLeft;
          ROS_INFO("Left");
          break;
        case 'd':
          msg.data = (char)dirRight;
          ROS_INFO("Right");
          break;
        case 'w':
          msg.data = (char)dirUp;
          ROS_INFO("Up");
          break;
        case 's':
          msg.data = (char)dirDown;
          ROS_INFO("Down");
          break;
        }


      publisher.publish(msg);
      ros::spinOnce();
      loop_rate.sleep();;
    }


  return 0;
}

