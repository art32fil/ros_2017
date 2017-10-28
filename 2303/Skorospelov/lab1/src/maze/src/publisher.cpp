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
	std::cout << "Use M key to switch between FPS and MAP modes." << std::endl;
	move pass;
	while ((ch = getch()) != 'q')
	{
		switch(ch)
		{
		case 'w':
			pass.x = 0; pass.y = 1;	
			pub.publish(pass);
			break;
		case 's':
			pass.x = 0; pass.y = -1;
			pub.publish(pass);
			break;
		case 'a':
			pass.x = -1; pass.y = 0;
			pub.publish(pass);
			break;
		case 'd':
			pass.x = 1; pass.y = 0;
			pub.publish(pass);
			break;
		case 'm':
			pass.x = 0; pass.y = 2;
			pub.publish(pass);
			break;
		}
		ros::spinOnce();
	}
	getch();
	return 0;
}

