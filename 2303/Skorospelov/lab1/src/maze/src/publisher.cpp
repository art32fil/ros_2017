#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"

#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

typedef geometry_msgs::Pose2D move;

move gm(int x, int y) {
	move res;
	res.x = x;
	res.y = y;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "published");
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<geometry_msgs::Pose2D>("moves", 1);

	ros::Rate loop_rate(10);
	int ch;

	std::cout << "Hey, yo!" << std::endl;
	std::cout << "Use WASD to move around" << std::endl;
	std::cout << "Q for exit" << std::endl;
	while ((ch = getch()) != 'q')
	{
		switch(ch)
		{
		case 'w':
			pub.publish(gm(0, -1));
			break;
		case 's':
			pub.publish(gm(0, 1));
			break;
		case 'a':
			pub.publish(gm(-1, 0));
			break;
		case 'd':
			pub.publish(gm(1, 0));
			break;
		case 'm':
			pub.publish(gm(2, 0));
			break;
		}
		ros::spinOnce();
	}
	getch();
	return 0;
}

