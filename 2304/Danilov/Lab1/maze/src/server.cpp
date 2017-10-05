#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include "maze/maze.h"

Maze maze;

void chatterCallback(const geometry_msgs::Pose2D::ConstPtr& message)
{
    if(message->x < 0) {
        maze.goUp();
    }
    if(message->x > 0) {
        maze.goDown();
    }
    if(message->y < 0) {
        maze.goLeft();
    }
    if(message->y > 0) {
        maze.goRight();
    }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("key_topic", 1000, chatterCallback);
  ros::spin();

  return 0;
}
