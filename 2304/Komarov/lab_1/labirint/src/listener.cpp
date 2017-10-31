#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include "labirint/labirint.h"

Labirint labirint;

void chatterCallback(const geometry_msgs::Pose2D::ConstPtr& message)
{
    if(message->x == 6 && message->y ==6)
        labirint.cheeterWin();
    if(message->x < 0)
        labirint.goUp();
    if(message->x > 0)
        labirint.goDown();
    if(message->y < 0)
        labirint.goLeft();
    if(message->y > 0)
        labirint.goRight();
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("key_topic", 1000, chatterCallback);
  ros::spin();

  return 0;
}
