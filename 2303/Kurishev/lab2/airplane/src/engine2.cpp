#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"

#include <sstream>

std_msgs::Int8 number;
bool broken = true;
bool destroied = false;
ros::Timer timer;

void fCallback(const std_msgs::Int8::ConstPtr& msg)
{
  if (broken & !destroied){	 
	  if (msg->data == 2){
                  timer.stop();
		  broken = false;
		  number.data = msg->data;
		  ROS_INFO("egnine : %i, was repaired", msg->data);
	  }
  }
}

void callback(const ros::TimerEvent& event)
{
   destroied = true;
}

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "engine2");

  
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("/master", 1000);
  ros::Subscriber sub = n.subscribe("/worked_engines", 100, fCallback);
  ros::Rate loop_rate(0.1);


  timer = n.createTimer(ros::Duration(30), callback);

  std_msgs::String msg;
  std::stringstream ss;
  ss << "help engine2";

  while (ros::ok() & !destroied)
  {
   

    if(broken){
	    msg.data = ss.str();
	    ROS_INFO("%s", msg.data.c_str());
	    chatter_pub.publish(msg);
    }

    ros::spinOnce();

    loop_rate.sleep();
  }
  msg.data = "2 egnine was destroyed";
  chatter_pub.publish(msg);

  ROS_INFO("engine with number 2 was destroyed");
  ros::shutdown();
  
 
  return 0;
}
