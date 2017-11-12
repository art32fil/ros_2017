#include "ros/ros.h"
#include <turtlesim/Pose.h>
#include "getch.cpp"

const int KEYCODE_UP = 65;
const int KEYCODE_RIGHT = 67;
const int KEYCODE_DOWN = 66;
const int KEYCODE_LEFT = 68;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "teleop");
  ros::NodeHandle n;
  ros::Publisher position_botA = n.advertise<turtlesim::Pose>("posBotA", 1);
  ros::Publisher position_botB = n.advertise<turtlesim::Pose>("posBotB", 1);
  ros::Rate loop_rate(10);

  puts("Reading from keyboard");
  puts("---------------------------");
  puts("Use arrow keys to move the bot.");

  int key;
  double x_posBot = 0;
  double y_posBot = 0;

  while (ros::ok() && ((key = getch()))) {
    turtlesim::Pose msg;
    msg.x = x_posBot;
    msg.y = y_posBot;
    
    switch (key) {
      case KEYCODE_UP:
          ROS_INFO("Up");
          x_posBot -= 0.1;
          msg.x = x_posBot;
          break;
      case KEYCODE_RIGHT:
          ROS_INFO("Right");
          y_posBot += 0.1;
          msg.y = y_posBot;
          break;
      case KEYCODE_DOWN:
          ROS_INFO("Down");
          x_posBot += 0.1;
          msg.x = x_posBot;
          break;
      case KEYCODE_LEFT:
          ROS_INFO("Left");
          y_posBot -= 0.1;
          msg.y = y_posBot;
          break;
    }
    
    position_botA.publish(msg);
    msg.x = -4;
    msg.y = -4;
    position_botB.publish(msg);

    ros::spinOnce();
    loop_rate.sleep();
  }
  

  return 0;
}