#include "ros/ros.h"
#include "handyman/Repair.h"
#include <time.h>

#include "handyman/plain.h"
#include "kbhit.cpp"

bool actionUser(int detailCode) {
    ROS_INFO("Detail %s destroy", details[detailCode]);
    bool isRepaired = false;

    ROS_INFO("Enter number detail for repair him");
    ROS_INFO("Enter %d", detailCode);

    time_t start_time = time(NULL);
    
    //delay 5seconds for the user
    int delay = 5;

    init_keyboard(); //kbhit

    int key = 0;

    do {
        if (kbhit()) {
            key = readch(); //kbhit
            
            if (key == (detailCode + 48)) {
                ROS_INFO("Detail %s repair", details[detailCode]);

                close_keyboard(); //kbhit
                return true;
            }
        }
    } while (time(NULL) <= start_time + delay);
    
    ROS_INFO("TIME IS OVER");
    close_keyboard(); //kbhit
    return false;
}

bool repairDetail(handyman::Repair::Request &req,
        handyman::Repair::Response &res) {
            res.isRepair = actionUser(req.codeDetail);

            return true;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "master");

    ros::NodeHandle n;
    
    ros::ServiceServer service = n.advertiseService("repair_detail", repairDetail);
    ROS_INFO("Ready to repait details");
    ros::spin();

    return 0;
}