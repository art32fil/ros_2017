#include "ros/ros.h"
#include "lab2_c/DriveUnitStatus.h"
#include "lab2_c/RepairingSrv.h"

using namespace ros;
using namespace lab2_c;
using namespace std;

Publisher repair_pub;

bool repair(RepairingSrv::Request& req,
  RepairingSrv::Response& resp)
{
  DriveUnitStatus msg;
  msg.id = req.id;
  msg.name = "doesnt_matter";
  msg.status = 1;

  repair_pub.publish(msg);

  return true;
}

int main(int argc, char** argv)
{
  init(argc, argv, "repair_service");
  NodeHandle repair_node;
  
  repair_pub = repair_node.advertise<DriveUnitStatus>("drive_unit_change_status", 1000);
  ServiceServer repair_service = repair_node.advertiseService("repair_drive_unit", repair);
  
  spin();

  return 0;
}
