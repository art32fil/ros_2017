#include "ros/ros.h"
#include "lab2_c/DriveUnitStatus.h"
#include <random>

using namespace ros;
using namespace std;
using namespace lab2_c;

int main(int argc, char** argv)
{
  random_device rd;     // only used once to initialise (seed) engine
  mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
  uniform_int_distribution<int> uni_drive_id(0, 5);
  uniform_int_distribution<int> uni_sleep_duration(2, 10);

  init(argc, argv, "trixxxter");
  NodeHandle trixter_node;
  
  Publisher trick_pub = trixter_node.advertise<DriveUnitStatus>("drive_unit_change_status", 1000);
  DriveUnitStatus msg;
  msg.name = "doesnt_matter";
  msg.status = 0;

  while (ok())
  {
    msg.id = uni_drive_id(rng);
    trick_pub.publish(msg);
    Duration(uni_sleep_duration(rng)).sleep();

    spinOnce();
  }
  
  return 0;
}
