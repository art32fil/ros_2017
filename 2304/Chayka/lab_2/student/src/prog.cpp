#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_srvs/Empty.h"
#include <sstream>
#include <fstream>

bool passed = true;

bool pass(std_srvs::Empty::Request  &req,
         std_srvs::Empty::Response &res){
  passed = true;
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "prog");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("debts", 1);
  ros::Publisher last_word = n.advertise<std_msgs::String>("expel", 1);
  ros::ServiceServer service = n.advertiseService("prog/pass_the_exam", pass);
  sleep(1);
  ros::Rate loop_rate(10);
  std::ifstream ifs("/dev/urandom", std::ifstream::in);
  std::srand(ifs.get());
  ifs.close();
  std_msgs::String msg;
  msg.data = "prog";
  while (ros::ok() && passed)
  {
    ROS_WARN("I'm prog and I'm dying (to fix call prog/pass_the_exam)");
    chatter_pub.publish(msg);
    passed = false;
    int border = 10 * (10 + std::rand()%30);
    for(int i=0;i<border;i++){
      ros::spinOnce();
      if(!ros::ok())
        break;
      loop_rate.sleep();
    }
  }
  last_word.publish(msg);
  return 0;
}
