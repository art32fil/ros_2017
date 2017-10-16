#include "ros/ros.h"
#include "handyman/AddTwoInts.h"
#include "handyman/Repair.h"

#include "handyman/plain.h"
#include <time.h>

details faultGeneration() {
    details brokenDetail;

    srand( time(0));

    int randomDetail = rand() % 6;

    switch (randomDetail) {
    case 0: brokenDetail = wing_1;
        break;
    case 1: brokenDetail = wing_2;
        break;
    case 2: brokenDetail = fuselage;
        break;
    case 3: brokenDetail = chassis;
        break;
    case 4: brokenDetail = tail;
        break;
    case 5: brokenDetail = pilot;
        break;
    }
    
    return brokenDetail;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "plainSystem");

    Plain plain;

    ros::NodeHandle n;

    ros::ServiceClient client = n.serviceClient<handyman::Repair>("repair_detail");

    handyman::Repair srv;

    ros::Rate loop_rate(10);

    ROS_INFO("RDY");

    details brokenDetail;
    while (ros::ok()) {
        if (!plain.checkCriticalCondition()) {
            ROS_INFO("Plain destoyed");
            return 0;
        }

        ROS_INFO("SIZE = %u", plain.getBrokeDetails().size());
        brokenDetail = faultGeneration();

        if (plain.destroyDetail(brokenDetail)) {
            srv.request.codeDetail = brokenDetail;

            if (client.call(srv)) {
                ROS_INFO("Response %d", srv.response.isRepair);
            }

            ROS_INFO("faultDetail: %d", brokenDetail);
            ros::Duration(3).sleep();
        }
    }


    ros::spinOnce();
    return 0;
}

