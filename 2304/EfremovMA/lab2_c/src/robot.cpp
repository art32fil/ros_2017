#include "ros/ros.h"
#include "lab2_c/MovingSrv.h"
#include "lab2_c/LocatingSrv.h"
#include "lab2_c/PickupingSrv.h"
#include "lab2_c/RepairingSrv.h"
#include "lab2_c/RobotControl.h"
#include "lab2_c/Coords.h"
#include "lab2_c/DriveUnitStatus.h"

using namespace ros;
using namespace lab2_c;
using namespace std;

/*=====data structs=====*/
struct Location {
  int x;
  int y;
  int west;
  int east;
  int north;
  int south;
  int current;
};

/*=====ros::node data=====*/
Subscriber controller;
Subscriber drives_status;
Publisher  robot_location;

ServiceClient locating_service;
ServiceClient moving_service;
ServiceClient pickup_service;
ServiceClient repair_service;
MovingSrv moving_service_pair;

/*=====robot's data fields=====*/
Location location;
int details_count;
bool drive_units_statuses[6];

/*=====utility functions=====*/
void convert_moving_service_request()
{
  moving_service_pair.request.x = location.x;
  moving_service_pair.request.y = location.y;
}

void convert_moving_service_response()
{
  location.west = moving_service_pair.response.west;
  location.east = moving_service_pair.response.east;
  location.north = moving_service_pair.response.north;
  location.south = moving_service_pair.response.south;
  location.current = moving_service_pair.response.current;
}

void make_move()
{
  convert_moving_service_request();
  moving_service.call(moving_service_pair);
  convert_moving_service_response();
}

/*=====robot's behavior=====*/
void move_west()
{
  if (location.west > 0)
  {
    location.x--;
    make_move();
  } else 
  {
    ROS_WARN("I can't go to WEST, there WALL is");
  }
}

void move_east()
{
  if (location.east > 0)
  {
    location.x++;
    make_move();
  } else 
  {
    ROS_WARN("I can't go to EAST, there WALL is");
  }
}

void move_north()
{
  if (location.north > 0)
  {
    location.y--;
    make_move();
  } else 
  {
    ROS_WARN("I can't go to NORTH, there WALL is");
  }
}

void move_south()
{
  if (location.south > 0)
  {
    location.y++;
    make_move();
  } else 
  {
    ROS_WARN("I can't go to SOUTH, there WALL is");
  }
}

void pickup_detail()
{
  if (location.current == 3)
  {
    PickupingSrv pickup_srv;
    pickup_srv.request.x = location.x;
    pickup_srv.request.y = location.y;

    pickup_service.call(pickup_srv);
    details_count++;
    ROS_WARN("I have %d details!", details_count);
  } else
  {
    ROS_WARN("I can catch NOTHING!");
  }
}

bool is_drive_ok(int id)
{
  if (0 <= id && id < 6)
    return drive_units_statuses[id];
  else
  {
    ROS_ERROR("drive_id = %d doesn't exists!!", id);
    return false;
  }
}

bool set_drive_unit_status(int id, bool is_ok)
{
  if (0 <= id && id < 6)
  {
    drive_units_statuses[id] = is_ok;
    return true;
  } else
  {
    ROS_ERROR("drive_id = %d doesn't exists!!", id);
    return false;
  }

}

void repair_drive()
{
  int id = -1;
  for (int i = 0; i < 6; i++)
  {
    if (drive_units_statuses[i] == false)
    {
      id = i;
      break;
    }
  }

  ROS_INFO("Broken_id = %d Details = %d", id, details_count);

  if (id >= 0)
  {
    if (details_count > 0)
    {
      details_count--;
      RepairingSrv srv;
      srv.request.id = id;
      repair_service.call(srv);
    } else
    {
      ROS_WARN("I have no DETAILS!! DETAILS REQUIRED!!");
    }
  } else
  {
    ROS_WARN("Nothing to repair. DONT BLINK! ESCAPE!!");
  }

}

/*=====callbacks=====*/
void get_control(const RobotControl::ConstPtr& msg)
{
  int code = msg->code;
  switch (code)
  {
    case 0:
    {
      move_north();
      break;
    }
    case 1:
    {
      move_east();
      break;
    }
    case 2:
    {
      move_south();
      break;
    }
    case 3:
    {
      move_west();
      break;
    }
    case 4:
    {
      pickup_detail();
      break;
    }
    case 5:
    {
      ROS_INFO("Try to repair..");
      repair_drive();
      break;
    }
  }
}

void analyse_drive_status(const DriveUnitStatus::ConstPtr& msg)
{
  string name = msg->name;
  if (!set_drive_unit_status(msg->id, msg->status))
    return;

  transform(name.begin(), name.end(), name.begin(), ::toupper);
  if (msg-> status == 0)
  {
    ROS_INFO("A-A-A-A-A-A, my %s was broken!!1 REPAIR! REPAIR!", name.c_str());
  } else
  {
    ROS_INFO("%s is REPAIRED! Continue to escape, ESCAPE!", name.c_str());
  }
}

/*=====MAIN=====*/
int main(int argc, char** argv)
{
  for (int i = 0; i < 6; i++)
  {
    drive_units_statuses[i] = true;
  }

  init(argc, argv, "good_poor_dalek");
  NodeHandle robot_node;

  locating_service = robot_node.serviceClient<LocatingSrv>("locate_robot");
  LocatingSrv location_srv;
  while (!locating_service.call(location_srv));

  location.x = location_srv.response.x;
  location.y = location_srv.response.y;
  location.west = location_srv.response.west;
  location.east = location_srv.response.east;
  location.north = location_srv.response.north;
  location.south = location_srv.response.south;
  location.current = location_srv.response.current;

  details_count = 0;

  pickup_service = robot_node.serviceClient<PickupingSrv>("pickup_detail");
  moving_service = robot_node.serviceClient<MovingSrv>("move_robot");
  repair_service = robot_node.serviceClient<RepairingSrv>("repair_drive_unit");
  controller = robot_node.subscribe("robot_control", 1000, get_control);
  drives_status = robot_node.subscribe("drive_units_status", 1000, analyse_drive_status);
  
  Rate loop_rate(10);
  while(ok() && location.current != 2)
  {
    spinOnce();
    loop_rate.sleep();
  }

  ROS_INFO("Dalek ESCAPED!");

//spin();

  return 0;
}
