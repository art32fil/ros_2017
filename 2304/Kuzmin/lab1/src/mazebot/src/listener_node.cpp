#include "ros/ros.h"
#include "std_msgs/Byte.h"
#include "../include/bot.cpp"

Bot b;
bool won;
ros::NodeHandle* n;
ros::Subscriber subscriber;

void publisherCallback(const std_msgs::Byte::ConstPtr& msg)
{
  switch (msg->data)
    {
    case dirLeft:
      b.MoveLeft();
      break;
    case dirRight:
      b.MoveRight();
      break;
    case dirUp:
      b.MoveUp();
      break;
    case dirDown:
      b.MoveDown();
      break;
    }

  b.DrawAll();

  if (b.CheckIfWon())
    {
      subscriber.shutdown();
      ROS_INFO("You won!");
    }
}

int main(int argc, char** argv)
{
  b.DrawAll();
  ros::init(argc, argv, "mazebot_node");
  n = new ros::NodeHandle();
  subscriber = n->subscribe("mazebot_cmds_topic", 1000, publisherCallback);
  ros::spin();
  delete n;
  return 0;
}
