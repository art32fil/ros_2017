#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include <ncurses.h>
#include <sstream>

#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif


int main(int argc, char **argv)
{
  ros::init(argc, argv, "master_node");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Pose2D>("orders", 1);

  ros::Rate loop_rate(10);
  int ch;

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  printw("Hello, Lord!\n");
  printw("Use arrows to manipulate a pathetic slave\n");
  printw("Ctrl+c to exit\n");
  while ((ch = getch()) != CTRL('c'))
  {
    geometry_msgs::Pose2D msg;
    std::stringstream ss;

    switch(ch)
      {
        case KEY_UP:
          ss << "KEY_UP";
          msg.x = -1;
          msg.y = 0;
          chatter_pub.publish(msg);
          break;
        case KEY_DOWN:
          ss << "KEY_DOWN";
          msg.x = 1;
          msg.y = 0;
          chatter_pub.publish(msg);
          break;
        case KEY_LEFT:
          ss << "KEY_LEFT";
          msg.x = 0;
          msg.y = -1;
          chatter_pub.publish(msg);
          break;
        case KEY_RIGHT:
          ss << "KEY_RIGHT";
          msg.x = 0;
          msg.y = 1;
          chatter_pub.publish(msg);
          break;
      }
    ros::spinOnce();
  }
  printw("Farewell, lord!\n");

  refresh();
  getch();
  endwin();

  return 0;
}
