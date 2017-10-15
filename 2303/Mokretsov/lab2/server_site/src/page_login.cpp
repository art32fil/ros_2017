#include "ros/ros.h"
#include "std_msgs/String.h"

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
	ros::init(argc, argv, "page_login");

	ros::NodeHandle n;

	ros::Publisher http = n.advertise<std_msgs::String>("http", 10);
	ros::Publisher httpFail = n.advertise<std_msgs::String>("httpFail", 10);

	std_msgs::String message;

	sleep(5);

	message.data = "page_login";

	http.publish(message);

	return 0;
}


