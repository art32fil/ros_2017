#include "ros/ros.h"
#include "fixrocket/repair_detail.h"
#include "fixrocket/repair_all.h"
#include "../include/details.cpp"

int repairsCount = 0;

bool repairCallback(fixrocket::repair_detail::Request& req,
    fixrocket::repair_detail::Response& res)
{
    if (req.data.detail > 0 && req.data.detail < detailscount && details[req.data.detail] != true) {
        details[req.data.detail] = true;
        repairsCount++;
        res.data.result = true;
        //ROS_INFO("Detail %d (%s) repaired", req.data.detail, detailsNames[req.data.detail]);
    }
    else {
        res.data.result = false;
    }
    return true;
}

bool repairAllCallback(fixrocket::repair_all::Request& req,
    fixrocket::repair_all::Response& res)
{
    ROS_INFO("It will set your repairs count to zero");
    repairsCount = 0;
    repairAll();
    return true;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "rocket_node");
    ros::NodeHandle n;
    ros::Rate loop_rate(1);

    std::string rocketName = ros::this_node::getName();

    ros::ServiceServer service = n.advertiseService(rocketName + "/repair_detail", repairCallback);
    ros::ServiceServer service2 = n.advertiseService(rocketName + "/repair_all", repairAllCallback);
    ROS_INFO("Try to repair broken details using repairer_node or rocket will be destroyed!");

    repairAll();

    int waitsec = 1;
    int i = 0;

    while (checkRocket()) {

        if (waitsec <= i) {
            waitsec = rand() % 5 + 5;
            i = 0;
            char workingDetail = findAnyWorking();
            details[workingDetail] = false;
            ROS_INFO("Warning! Detail %d (%s) has been broken. Repair it quickly!", workingDetail, detailsNames[workingDetail]);
        }

        ros::spinOnce();
        loop_rate.sleep();
        i++;
    }

    ROS_INFO("Game over. Your repairs count: %d", repairsCount);

    return 0;
}
