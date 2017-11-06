#include "ros/ros.h"
#include <ncurses.h>
#include <sstream>
#include <turtlesim/Pose.h>


int main(int argc, char **argv)
{
  ros::init(argc, argv, "keys");
  ros::NodeHandle n;
  ros::Publisher publisherA = n.advertise<turtlesim::Pose>("poseA", 1);
  ros::Publisher publisherB = n.advertise<turtlesim::Pose>("poseB", 1);
  double x = 0;
  double y = 0;
  ros::Rate loop_rate(10);
  int ch;

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  timeout(25);
printw("$ - is you, # - is exit\nManipulating with arrows\nGood luck, have fun\nPress 'Backspace' for exit n\n");
  while ((ch = getch()) !=  KEY_BACKSPACE)
  {
    turtlesim::Pose msg;
    msg.x = x;
    msg.y = y;

    switch(ch)
      {
        case KEY_UP:
          x -= 0.1;
          msg.x = x;
          break;
        case KEY_DOWN:
          x += 0.1;
          msg.x = x;
          break;
        case KEY_LEFT:
          y -= 0.1;
          msg.y = y;
          break;
        case KEY_RIGHT:
          y += 0.1;
          msg.y = y;
          break;
      }
    publisherA.publish(msg);
    msg.x = -4;
    msg.y = -4;
    publisherB.publish(msg);
    ros::spinOnce();
  }

  refresh();
  getch();
  endwin();

  return 0;
}
