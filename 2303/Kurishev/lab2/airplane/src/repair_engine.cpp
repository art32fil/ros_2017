#include "ros/ros.h"
#include "airplane/repair_engine.h"
#include <cstdlib>
#include "std_msgs/Int8.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "repair_engine");
  if (argc != 2)
  {
    ROS_INFO("usage: number (number of engine, 1-5)");
    return 1;
  }

  std_msgs::Int8 msg;

  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<std_msgs::Int8>("/worked_engines", 100);
  

  ros::ServiceClient client = n.serviceClient<airplane::repair_engine>("repair_engine");
  airplane::repair_engine srv;

  srv.request.n = atoll(argv[1]);
  msg.data = srv.request.n;  

  if (client.call(srv))
  {
    while(pub.getNumSubscribers() == 0){
    ROS_ERROR("waiting subs...");
    sleep(1);
    }
    pub.publish(msg);
    ROS_INFO("Try to repair engine with number : %ld", (long int)srv.response.b);
  }
  else
  {
    ROS_ERROR("Failed to call service repair_engine");
    return 1;
  }

  return 0;
}
