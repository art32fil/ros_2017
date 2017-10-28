#include "ros/ros.h"
#include <treasurehunter/Pose.h>
#include "treasurehunter/mazeframe.h"

MazeFrame maze;

void moveHunterCallback(const treasurehunter::PoseConstPtr& pos) {
    bool isSuccessfully = true;

    switch (pos->key) {
        case 0:
            isSuccessfully = maze.moveUp();
            break;
        case 1:
            isSuccessfully = maze.moveRight();
            break;
        case 2:
            isSuccessfully = maze.moveDown();
            break;
        case 3:
            isSuccessfully = maze.moveLeft();
            break;
    }

    if (!isSuccessfully) {
        ROS_INFO("Oops! This is Wall!");
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "maze_listener");
    ros::NodeHandle n;
    ros::Subscriber sub_moveHunter = n.subscribe("moveHunter_topic", 1000, moveHunterCallback);
    
    ros::spin();

    return 0;
}