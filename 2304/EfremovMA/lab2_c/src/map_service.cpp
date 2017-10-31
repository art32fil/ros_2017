#include "ros/ros.h"
#include "lab2_c/MovingSrv.h"
#include "lab2_c/LocatingSrv.h"
#include "lab2_c/PickupingSrv.h"
#include <stdio.h>
#include <stdlib.h>

using namespace ros;
using namespace lab2_c;

struct Coords
{
  int x;
  int y;
};

char map[10][10] = {
  {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
  {'#', '.', '#', '.', 'd', '.', '#', '.', 'd', '#'},
  {'#', '.', '#', '.', '#', '#', '#', '.', '#', '#'},
  {'#', 'd', '#', '.', '.', '.', '.', '.', '.', '#'},
  {'#', '.', '.', '.', '#', '#', '#', '.', '#', '#'},
  {'#', '.', '#', '.', '#', '.', '.', '.', '#', '#'},
  {'#', '.', '#', '.', '#', '.', '#', '.', '.', '#'},
  {'#', '.', '.', 'd', '#', '.', '#', '.', '#', '#'},
  {'#', '.', '#', '.', '#', 'f', '#', '.', 'd', '#'},
  {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

Coords robot_location;

void print_map()
{
  system("clear");
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++) 
    {
      if (j == robot_location.x &&
        i == robot_location.y)
      {
        printf("R ");
      } else
      {
        printf("%c ", map[i][j]);
      }
    }
    printf("\n");
  }
}

int convert_cell_to_code(int x, int y)
{
  switch (map[y][x])
  {
    case '#':
      return 0;
    case '.':
      return 1;
    case 'f':
      return 2;
    case 'd':
      return 3;
    default:
      return -1;
  }
}

bool locate_robot(LocatingSrv::Request &req,
  LocatingSrv::Response &resp)
{
  int x = robot_location.x;
  int y = robot_location.y;

  resp.x = x;
  resp.y = y;

  resp.west = convert_cell_to_code(x - 1, y);
  resp.east = convert_cell_to_code(x + 1, y);
  resp.north = convert_cell_to_code(x, y - 1);
  resp.south = convert_cell_to_code(x, y + 1);
  resp.current = convert_cell_to_code(x, y);

  return true;
}

bool move_robot(MovingSrv::Request &req,
  MovingSrv::Response &resp)
{
  int x = req.x;
  int y = req.y;

  robot_location.x = x;
  robot_location.y = y;

  resp.west = convert_cell_to_code(x - 1, y);
  resp.east = convert_cell_to_code(x + 1, y);
  resp.north = convert_cell_to_code(x, y - 1);
  resp.south = convert_cell_to_code(x, y + 1);
  resp.current = convert_cell_to_code(x, y);

  print_map();

  return true;
}

bool pickup_detail(PickupingSrv::Request &req,
  PickupingSrv::Response &resp)
{
  map[req.y][req.x] = '.';
  return true;
}

int  main(int argc, char** argv)
{
  init(argc, argv, "map_service");
  NodeHandle map_service_node;

  robot_location.x = 1;
  robot_location.y = 8;
  print_map();

  ServiceServer locate_robot_service = 
    map_service_node.advertiseService("locate_robot", locate_robot);

  ServiceServer move_robot_service = 
    map_service_node.advertiseService("move_robot", move_robot);

  ServiceServer pickup_service = 
    map_service_node.advertiseService("pickup_detail", pickup_detail);

  spin();

  return 0;
}
