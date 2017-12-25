#include "ros/ros.h"
#include <ncurses.h>
#include <sstream>
#include <turtlesim/Pose.h>

#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif


int main(int argc, char **argv)
{
  ros::init(argc, argv, "keys");
  ros::NodeHandle n;
  ros::Publisher chatter_pubA = n.advertise<turtlesim::Pose>("poseA", 1);
  ros::Publisher chatter_pubB = n.advertise<turtlesim::Pose>("poseB", 1);
  double x = 0;
  double y = 0;
  ros::Rate loop_rate(10);
  int ch;

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  timeout(25);
  printw("Use arrows to manipulate a robot\n");
  printw("Ctrl+c to exit\n");
  while ((ch = getch()) != CTRL('c'))
  {
    turtlesim::Pose msg;
    msg.x = x;
    msg.y = y;
    std::stringstream ss;

    switch(ch)
      {
        case KEY_UP:
          ss << "KEY_UP";
          x -= 0.1;
          msg.x = x;
          break;
        case KEY_DOWN:
          ss << "KEY_DOWN";
          x += 0.1;
          msg.x = x;
          break;
        case KEY_LEFT:
          ss << "KEY_LEFT";
          y -= 0.1;
          msg.y = y;
          break;
        case KEY_RIGHT:
          ss << "KEY_RIGHT";
          y += 0.1;
          msg.y = y;
          break;
      }
    chatter_pubA.publish(msg);
    msg.x = -4;
    msg.y = -4;
    chatter_pubB.publish(msg);
    ros::spinOnce();
  }
  printw("Bye bye\n");

  refresh();
  getch();
  endwin();

  return 0;
}
