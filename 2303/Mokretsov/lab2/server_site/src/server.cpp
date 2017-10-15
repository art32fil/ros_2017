#include "ros/ros.h"
#include "std_msgs/String.h"

std::set<std::string> failPages;

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void errorPage(const std_msgs::String::ConstPtr& msg)
{
	ROS_INFO("Page error detected = %s", msg->data.c_str());
}

void failRepairPage(const std_msgs::String::ConstPtr& msg)
{
	failPages.insert(msg->data);
	if (failPages.size() > 1) {
		ROS_INFO("Game Over");
		ros::shutdown();
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "server");

	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("http", 10, errorPage);

	ros::spin();

	return 0;
}

