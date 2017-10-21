#include "ros/ros.h"
#include "lab2_c/RobotControl.h"

using namespace ros;

bool is_escaped;

int keystroke_to_code(char c)
{
  switch (c)
  {
    case 'w':
      return 0;
    case 'd':
      return 1;
    case 's':
      return 2;
    case 'a':
      return 3;
    case 'c':
      return 4;
    case 'r':
      return 5;
    default:
      return -1;
  }
}

int main(int argc, char** argv)
{
  is_escaped = false;

  init(argc, argv, "robot_controller");
  NodeHandle controller_node;

  Publisher robot_controller = 
    controller_node.advertise<lab2_c::RobotControl>("robot_control", 1000);

  lab2_c::RobotControl control_msg;
  char keystroke;
  while(ok())
  {
    system("/bin/stty raw");
    keystroke = tolower(getchar());
    system("/bin/stty cooked");

    if (keystroke == 'q'){
      break;
    }

    control_msg.code = keystroke_to_code(keystroke);
    robot_controller.publish(control_msg);

    spinOnce();
  }

  return 0;
}
