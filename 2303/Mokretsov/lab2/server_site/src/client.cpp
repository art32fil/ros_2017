#include "ros/ros.h"
#include "server_site/SrvMessage.h"
#include "server_site/Page.h"
#include <cstdlib>

std::set<int> failPage;
std::string pageNames[6] = {
	"index.html",
	"login.html",
	"news.html",
	"blog.html",
	"cabinet.html",
	"shop.html"
};

bool repair_page(server_site::SrvMessage::Request  &req,
         server_site::SrvMessage::Response &res)
{
	int page = req.page;
	if (failPage.count(page) > 0) {
		res.result = pageNames[page];
		failPage.erase(page);
	} else {
		ROS_INFO("This page is normal working");
	}
	return true;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "client");

	ros::NodeHandle n;

	ros::ServiceServer service = n.advertiseService("repair_page", repair_page);
	ros::Publisher http = n.advertise<server_site::Page>("http", 10);

	server_site::Page message;

	ros::Rate loop_rate(0.2);

	int count = rand() % 6;
	while (ros::ok()) {
		while (failPage.count(count) > 0) {
			count = rand() % 6;
		}
		failPage.insert(count);
		if (failPage.size() > 4) {
			ROS_WARN("Site broke ^_^");
			ros::shutdown();
		}
		message.page_name = pageNames[count];
		message.page_number = count;
		ROS_INFO("Error page : %s, %ld", message.page_name.c_str(), (long int) count);
		http.publish(message);
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}


