#include "ros/ros.h"
#include "fixrocket/repair_detail.h"
#include "fixrocket/repair_all.h"
#include <cstdlib>
#include "../include/details.cpp"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "repairer_node");
    ROS_INFO("Enter rocket name (default is rocket_node):");
    std::string rocketName;
    std::cin >> rocketName;

    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<fixrocket::repair_detail>(rocketName + "/repair_detail");
    fixrocket::repair_detail srv;

    ROS_INFO("To repair detail enter its ID and press enter.");
    while (true) {
        int detailId;
        std::cin >> detailId;
        srv.request.data.detail = detailId;

        if (client.call(srv)) {
            if (srv.response.data.result == true)
                ROS_INFO("Detail %d (%s) repaired", srv.request.data.detail,
                    detailsNames[srv.request.data.detail]);
            else
                ROS_INFO("Error of repairing detail");
        }
        else {
            ROS_ERROR(
                "Can't found rocket with this name. Maybe it was already destroyed?");
            return 1;
        }
        ros::spinOnce();
    }

    return 0;
}
