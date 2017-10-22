#include "ros/ros.h"
#include "DyingHumanSimulator/healing.h"
#include "../include/organs.cpp"

int healedCount = 0;

bool repairCallback(DyingHumanSimulator::healing::Request& req,
    DyingHumanSimulator::healing::Response& res)
{
    if (req.data.organ > 0 && req.data.organ < 6 && organs[req.data.organ] != true) {
       organs[req.data.organ] = true;
        healedCount++;
        res.data.result = true;        
    }
    else {
        res.data.result = false;
    }
    return true;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "human_node");
    ros::NodeHandle n;
    ros::Rate loop_rate(1);

    std::string humanName = ros::this_node::getName();

    ros::ServiceServer service = n.advertiseService(humanName + "/healing", repairCallback); 
    //ROS_INFO("Try to repair broken organs using repairer_node or rocket will be destroyed!");
	ROS_INFO("He start dying!! Try to save his life! Heal his organs ffs!");

    initHuman();

    int waitsec = 1;
    int i = 0;

    while (checkHuman()) {

        if (waitsec <= i) {
            waitsec = rand() % 5 + 5;
            i = 0;
            char normalOrgan = findAnyAlive();
            organs[normalOrgan] = false;
            ROS_INFO("Quick! His organ %d (%s) disabled. Heal it OR HE WILL DIE", normalOrgan, organsNames[normalOrgan]);
        }

        ros::spinOnce();
        loop_rate.sleep();
        i++;
    }

    ROS_INFO("Its too late... He is dead... Your healed count: %d", healedCount);

    return 0;
}
