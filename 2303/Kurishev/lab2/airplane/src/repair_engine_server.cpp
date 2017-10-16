#include "ros/ros.h"
#include "airplane/repair_engine.h"

bool repair(airplane::repair_engine::Request  &req,
        airplane::repair_engine::Response &res)
{
  res.b = req.n;
  ROS_INFO("request: number = %ld", (long int)req.n);
  ROS_INFO("sending back response: [%ld]", (long int)res.b);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "repair_engine_server");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("repair_engine", repair);
  ROS_INFO("Ready to repair.");
  ros::spin();

  return 0;
}
