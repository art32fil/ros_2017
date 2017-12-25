#include "ros/ros.h"
#include "DyingHumanSimulator/healing.h"
#include <cstdlib>
#include "../include/organs.cpp"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "healing_node");    
    std::string humanName = "human_node";
    

    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<DyingHumanSimulator::healing>(humanName + "/healing");
    DyingHumanSimulator::healing srv;

    ROS_INFO("To heal enter its ID.");
    while (true) {
        int organId;
        std::cin >> organId;
        srv.request.data.organ = organId;

        if (client.call(srv)) {
            if (srv.response.data.result == true)
                ROS_INFO("Organ %d (%s) is OK for now", srv.request.data.organ,
                    organsNames[srv.request.data.organ]);
            else
                ROS_INFO("Its already healed");
        }
        else {
            ROS_ERROR(
                "WE CAN'T FIND HIM, WE THINK HE ALREADY DEAD. It's not our fault...");
            return 1;
        }
        ros::spinOnce();
    }

    return 0;
}
