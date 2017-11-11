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
  double bx = -3;
  double by = -3;
  double randbx = 0;
  double randby = 0;
  turtlesim::Pose msg;
  turtlesim::Pose bmsg;
  bmsg.x = bx;
  bmsg.y = by;
  msg.x = x;
  msg.y = y;
  int waitsec = 30;
  int i = 0;
  while ((ch = getch()) !=  KEY_BACKSPACE)
  {
    if (waitsec <= i) {
        i = 0;
        randbx = (((double) rand() / (RAND_MAX)) - 0.5)/10;
        randby = (((double) rand() / (RAND_MAX)) - 0.5)/10;
    }
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
    bx += randbx;
    bmsg.x = bx;
    by += randby;
    bmsg.y = by;
    publisherB.publish(bmsg);

    ros::spinOnce();
    i++;
  }

  refresh();
  getch();
  endwin();

  return 0;
}
