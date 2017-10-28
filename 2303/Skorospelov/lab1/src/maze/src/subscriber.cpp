#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"

#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include <time.h>
#include <limits>
const static int N = 20;

class Maze {
	private:
		std::vector<std::string> data;
		int state; //0 - map, 1 - FPS
		int px;
		int py;
		int dir; //0 - right, 1 - up, 2 - left, 3 - down
		int tx;
		int ty;
		int switches;
		int dirD[4][2] = {
			{ 0, 1 },
			{ -1, 0 },
			{ 0, -1 },
			{ 1, 0 }
		};
	public:
		Maze() {
			srand(time(NULL));
			init();
		};
		void init() {
			for (int i = 0; i < N; i++) {
				data.push_back(std::string(N, '.'));
			}
			for (int i = 0; i < N*N/4; i++) {
				int x = rand() % N;
				int y = rand() % N;
				data[y][x] = 'X';
			}
			px = rand() % N;
			py = rand() % N;
			while (tx == px && ty == py) {
				tx = rand() % N;
				ty = rand() % N;
			}
			data[ty][tx] = 'T';
			state = 0;
			dir = rand() % 4;
			//data[py][px] = '@';
			switches = 0;
		}
		void switchState() {
			state ^= 1;
			switches++;
		};
		bool inBounds(int val) {
			return (val >= 0 && val < N);
		};
		bool isValidMoveTo(int y, int x) {
			return inBounds(y) && inBounds(x) && data[y][x] != 'X';
		};
		void drawMap() {	
			std::cout << "[MAP]:" << std::endl;
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < data[i].size(); j++) {
					//MORE HARDCORE
					//if (i == py && j == px) {
					//	std::cout << '@';
					//}
					//else {
						std::cout << data[i][j];
					//}
				}
				std::cout << std::endl;
			}
		};
		void drawFPS() {
			std::cout << "[FPS]:" << std::endl;
			switch (dir) {
				case 0:
					//right
					if (px == N - 1) {
						std::cout << "You see nothing" << std::endl;
					}
					else if (data[py][px + 1] == 'X') {
						std::cout << "You see wall in front of you" << std::endl;
					}
					else {
						std::cout << "You see nice ground" << std::endl;
					}
					break;
				case 1:
					//up
					if (py == 0) {
						std::cout << "You see nothing" << std::endl;
					}
					else if (data[py - 1][px] == 'X') {
						std::cout << "You see wall in front of you" << std::endl;
					}
					else {
						std::cout << "You see nice ground" << std::endl;
					}
					break;
				case 2:
					//left
					if (px == 0) {
						std::cout << "You see nothing" << std::endl;
					}
					else if (data[py][px - 1] == 'X') {
						std::cout << "You see wall in front of you" << std::endl;
					}
					else {
						std::cout << "You see nice ground" << std::endl;
					}
					break;
				case 3:
					//down
					if (py == N - 1) {
						std::cout << "You see nothing" << std::endl;
					}
					else if (data[py + 1][px] == 'X') {
						std::cout << "You see wall in front of you" << std::endl;
					}
					else {
						std::cout << "You see nice ground" << std::endl;
					}
					break;
			}
		};
		void drawFinal() {
			std::cout << "OK, you got it! You looked at map " << switches / 2 << " times. Can you improve? Press D" << std::endl;
			init();
		};
		void drawCurrent() {
			if (state == 1) {
				drawFPS();
			}
			else if (state == 2) {
				drawMap();
				drawFinal();
			} else {
				drawMap();
			}
		};
		bool move(int y, int x) {
			//std::cout << "[DEBUG]: px=" << px << ", py=" << py << ", dir=" << dir << std::endl;
			if (y == 2) {
				switchState();
				return true;
			}
			if (state == 0) {
				std::cout << "You can't look at map and move at the same time, too easy" << std::endl;
				return false;
			}
			if (x == 1) {
				std::cout << "You turn right" << std::endl;
				dir = (dir - 1) % 4;
				if (dir < 0) {
					dir += 4;
				}
			}
			else if (x == -1) {
				std::cout << "You turn left" << std::endl;
				dir = (dir + 1) % 4;
			}
			if (y == 1) {
				if (isValidMoveTo(py + dirD[dir][0], px + dirD[dir][1])) {
					px += dirD[dir][1];
					py += dirD[dir][0];
					std::cout << "You move forward" << std::endl;
				}
				else {
					std::cout << "You can't move here" << std::endl;
				}
			}
			else if (y == -1) {
				if (isValidMoveTo(py - dirD[dir][0], px - dirD[dir][1])) {
					px -= dirD[dir][1];
					py -= dirD[dir][0];
					std::cout << "You move backwards" << std::endl;
				}
				else {
					std::cout << "You can't move here" << std::endl;
				}
			}
			if (py == ty && px == tx) {
				state = 2;
			}
			return true;
		};
};

Maze maze;

void chatterCallback(const geometry_msgs::Pose2D::ConstPtr& msg)
{
	for (int i = 0; i < 100; i++) {
		std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	}
	//ROS_INFO("Got: [%lf][%lf]", msg->x, msg->y);
	if (maze.move(msg->y, msg->x)) {
		maze.drawCurrent();
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "subscriber");
	ros::NodeHandle n;
	std::cout << "Get ready for REAL HARDCORE!" << std::endl;
	std::cout << "You can't actually solve this maze, can you?" << std::endl;
	maze.drawCurrent();
	ros::Subscriber sub = n.subscribe("moves", 1, chatterCallback);
	ros::spin();

	return 0;
}

