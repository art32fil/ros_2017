#include "ros/ros.h"
#include "bicycle/Repair.h"

int countSuccesFixedDetails = 0;

static const char* detailsNames[] = {
    "Front_wheel",
    "Back_wheel",
    "Helm",
    "Pedals",
    "Human"
};

const int detailscount = 5;

bool details[detailscount];

bool checkBicycle()
{
    int count = 0;
    for (int i = 0; i < detailscount; i++) {
        if (details[i] == false)
            count++;
        if(count > 2)
            return true;
    }
    return false;
}

char findAnyWorking()
{
    char cur;
    do {
        cur = rand() % detailscount;
    } while (details[cur] == true);
    return cur;
}

bool callback(bicycle::Repair::Request  &req,
         bicycle::Repair::Response &res)
{
    if (req.repairCode >= 0 && req.repairCode < detailscount) {
        details[req.repairCode] = false;
        countSuccesFixedDetails++;
        res.isSuccess = true;
        res.detail = detailsNames[req.repairCode];
        res.countRepairedDetails = countSuccesFixedDetails;
        ROS_INFO("%s repaired", detailsNames[req.repairCode]);
    }
    else {
        res.isSuccess = false;
    }
    return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "repair_server");
  ros::NodeHandle n;
  ros::Rate loop_rate(1);

  ros::ServiceServer service = n.advertiseService("repair", callback);
  ROS_INFO("Ready.");

  int waitsec = 5;
  int i = 0;

  while (checkBicycle() && countSuccesFixedDetails < 10) {

      if (waitsec <= i) {
          i = 0;
          char workingDetail = findAnyWorking();
          details[workingDetail] = true;
          ROS_INFO("Alyarma! %s crushed! Input %d", detailsNames[workingDetail], workingDetail);
      }

      ros::spinOnce();
      loop_rate.sleep();
      i++;
  }
  if(countSuccesFixedDetails >= 10)
    ROS_INFO("YOU WIN");
  else
    ROS_INFO("Game over. Your repairs count: %d", countSuccesFixedDetails);

  ros::spin();

  return 0;
}
