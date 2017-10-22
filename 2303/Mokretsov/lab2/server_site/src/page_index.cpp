#include "ros/ros.h"
#include "std_msgs/String.h"

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
	ros::init(argc, argv, "page_index");

	ros::NodeHandle n;

	ros::Publisher http = n.advertise<std_msgs::String>("http", 10);
	ros::Publisher httpFail = n.advertise<std_msgs::String>("httpFail", 10);

	std_msgs::String message;

	sleep(5);

	message.data = "page_index";

	http.publish(message);

	ros::spinOnce();

	return 0;
}

