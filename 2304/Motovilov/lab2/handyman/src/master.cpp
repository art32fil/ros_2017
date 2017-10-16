#include "ros/ros.h"
#include "handyman/AddTwoInts.h"
#include "handyman/Repair.h"

#include "handyman/plain.h"

bool repairDetail(handyman::AddTwoInts::Request &req,
        handyman::AddTwoInts::Response &res) {
            res.isRepair = repair(req.codeDetail);

            return true;
        }

bool repair(int detailCode) {
    ROS_INFO("Detail %d destroy", detailCode);

    //TODO: доделать данный кусок
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "master");

    ros::NodeHandle n;
    
    ros::ServiceServer service = n.advertiseService("repair_detail", repairDetail);
    ROS_INFO("Ready to repait details");
    ros::spin();

    return 0;
}