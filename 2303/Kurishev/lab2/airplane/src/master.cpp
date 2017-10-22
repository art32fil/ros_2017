#include "ros/ros.h"
#include "std_msgs/String.h"
#include <stdio.h>
#include <string.h>

class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    count_destroyed = 0;
    //str_temp[0] = '0';
    //Topic you want to publish
    pub = n.advertise<std_msgs::String>("/destroyed", 1000);
    //Topic you want to subscribe
    sub = n.subscribe("master", 1000,&SubscribeAndPublish::callback, this);

  }

  void callback(const std_msgs::String::ConstPtr& msg)
  {
	   if (count_destroyed<=3){ //<=3
	    	strncpy ( str_temp, msg->data.c_str(),1);
	    	if (str_temp[0] != 'h'){
 		ROS_INFO("%s",msg->data.c_str());
	        count_destroyed++;
	    	}
	        else{
	           ROS_INFO("Please open new terminal and use repair_engine to %s",msg->data.c_str());
	        }
          }
	  if (count_destroyed == 3){ 
	     ROS_INFO("AIRPLANE WAS DESTROYED");
	     count_destroyed ++;
	     std_msgs::String msg;
	     msg.data = "kill";
	     pub.publish(msg);
	  } 
  }

private:
  int count_destroyed;
  char str_temp[1];
  std_msgs::String msg;
  ros::NodeHandle n; 
  ros::Publisher pub;
  ros::Subscriber sub;

};//End of class SubscribeAndPublish

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "master");

  //Create an object of class SubscribeAndPublish that will take care of everything
  SubscribeAndPublish SAPObject;

  ros::spin();

  return 0;
}
