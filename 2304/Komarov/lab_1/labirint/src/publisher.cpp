#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"
#include <ncurses.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "publisher_node");
  ros::NodeHandle n;
  ros::Publisher keyPublisher = n.advertise<geometry_msgs::Pose2D>("key_topic", 1000);

  ros::Rate loop_rate(10);
  int ch;
  int counter = 3;

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  printw("$ - is you, # - is exit\nManipulating with arrows\nGood luck, have fun\nPress 'Backspace' for exit \n\nPLEASE, DON'T PRESS 'SPACE'\n\n");

  while ((ch = getch()) !=  KEY_BACKSPACE)
  {
    geometry_msgs::Pose2D message;

    switch(ch)
    {
        case ' ':
            counter--;
            printw("NO %d\n", counter);
            if(counter == 0)
            {
                message.x = 6;
                message.y = 6;
                keyPublisher.publish(message);
                counter = 3;
            }
            break;
        case KEY_UP:
            message.x = -1;
            message.y = 0;
            keyPublisher.publish(message);
            break;
        case KEY_DOWN:
            message.x = 1;
            message.y = 0;
            keyPublisher.publish(message);
            break;
        case KEY_LEFT:
            message.x = 0;
            message.y = -1;
            keyPublisher.publish(message);
            break;
        case KEY_RIGHT:
            message.x = 0;
            message.y = 1;
            keyPublisher.publish(message);
            break;
    }
    ros::spinOnce();
  }
  refresh();
  getch();
  endwin();

  return 0;
}
