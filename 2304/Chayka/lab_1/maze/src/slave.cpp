#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include "maze/maze.h"

Maze maze;

void chatterCallback(const geometry_msgs::Pose2D::ConstPtr& msg)
{
  //ROS_INFO("I heard: [%lf]", msg->x);
  if(msg->x < 0)
    maze.moveUp();
  if(msg->x > 0)
    maze.moveDown();
  if(msg->y < 0)
    maze.moveLeft();
  if(msg->y > 0)
    maze.moveRight();
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  maze.draw();
  ros::Subscriber sub = n.subscribe("orders", 1, chatterCallback);
  ros::spin();

  return 0;
}
