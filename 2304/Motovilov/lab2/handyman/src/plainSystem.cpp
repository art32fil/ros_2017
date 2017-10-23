#include "ros/ros.h"
#include "handyman/Repair.h"

#include "handyman/plain.h"
#include <time.h>

int faultGeneration() {
    srand(time(0));

    int randomDetail = rand() % 6;
  
    return randomDetail;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "plainSystem");

    Plain plain;

    ros::NodeHandle n;

    ros::ServiceClient client = n.serviceClient<handyman::Repair>("repair_detail");

    handyman::Repair srv;

    ROS_INFO("Ready to receive error messages");

    int brokenDetail;

    //delay to destroy
    int delay = 8;

    while (ros::ok()) {
        if (!plain.checkCriticalCondition()) {
            ROS_INFO("The plane collapsed");
            return 0;
        }

        brokenDetail = faultGeneration();

        if (plain.destroyDetail(details[brokenDetail])) {
            ROS_INFO("Detail %s has broken", details[brokenDetail]);

            srv.request.codeDetail = brokenDetail;

            if (client.call(srv)) {
                if (srv.response.isRepair) {
                    ROS_INFO("Master repaired the %s", details[brokenDetail]);
                    plain.repairDetail(details[brokenDetail]);
                }
            }

            ros::Duration(delay).sleep();

            ROS_INFO("Number of destroyed details: %d", plain.getBrokeDetails().size());
        }
    }

    ros::spinOnce();
    return 0;
}

