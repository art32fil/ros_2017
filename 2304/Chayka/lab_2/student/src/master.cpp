#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <set>
#include <string>


std::set<std::string> set;

void expel(const std_msgs::String::ConstPtr& msg)
{
  set.insert(msg->data);
  if(set.size()>=2){
    ROS_WARN("NOOOOOOOOOOOOOOOOOOOO!!!!!!!!!!!!!");
    ros::shutdown();
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "master");
  ros::NodeHandle n;
  ros::Subscriber sub1 = n.subscribe("expel", 10, expel);
  ros::spin();

  return 0;
}
