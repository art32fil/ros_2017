#include "ros/ros.h"
#include "lab2_c/DriveUnitStatus.h"
#include <string>

using namespace ros;
using namespace lab2_c;
using namespace std;

Publisher status_publisher;
Subscriber status_changer; 

bool is_ok;
string name;
int id;

int get_status()
{
  if (is_ok)
    return 1;
  else
    return 0;
}

void set_status(int s)
{
  if (s == 0){
    is_ok = false;
  } else
  {
    is_ok = true;
  }
}

void change_status(const DriveUnitStatus::ConstPtr& msg)
{
  if (msg->id == id && msg->status != get_status())
  {
    set_status(msg->status);

    DriveUnitStatus pub_msg;
    pub_msg.id = id;
    pub_msg.name = name;
    pub_msg.status = get_status();

    status_publisher.publish(pub_msg);
  }
}

int main(int argc, char** argv)
{
  init(argc, argv, "Drive_unit_0");
  NodeHandle drive_unit_node("~");

  drive_unit_node.param("id", id, 42);
  drive_unit_node.param<string>("name", name, "a_drive_unit");
  is_ok = true;

  ROS_INFO("id = %d | name = %s", id, name.c_str());

  status_publisher = drive_unit_node.advertise<DriveUnitStatus>("/drive_units_status", 1000);
  status_changer = drive_unit_node.subscribe("/drive_unit_change_status", 1000, change_status);

  spin();
  

  return 0;
}
