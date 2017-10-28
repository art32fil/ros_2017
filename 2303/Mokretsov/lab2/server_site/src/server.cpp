#include "ros/ros.h"
#include "server_site/SrvMessage.h"
#include "server_site/Page.h"

void error_page(const server_site::Page msg) {
	ROS_INFO("The client broke the page: [%s], number = %ld", msg.page_name.c_str(), (long int) msg.page_number);
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "server");

	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("http", 10, error_page);
	ros::ServiceClient client = n.serviceClient<server_site::SrvMessage>("repair_page");

	int str;
	server_site::SrvMessage message;

	while (ros::ok()) {
		ROS_INFO("Enter number of page for fix");
		std::cin >> str;
		//std::getline(std::cin,str);
		message.request.page = str;
		if (client.call(message))
		{
			ROS_INFO("Success repair page: %s", message.response.result.c_str());
		}
		else
		{
			ROS_ERROR("Failed to call service repair_page");
		}
		ros::spinOnce();
	}

	return 0;
}

