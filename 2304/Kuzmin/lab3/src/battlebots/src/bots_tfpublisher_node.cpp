#include <vector>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <tf2_ros/transform_broadcaster.h>
#include <gazebo_msgs/ModelStates.h>
#include <gazebo_msgs/LinkStates.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/TransformStamped.h>

using namespace ros;

template<class T>
void SendTransformStamped(std::string name, T msg) {

    static tf2_ros::TransformBroadcaster tfBroadcaster;
    geometry_msgs::TransformStamped ts;

    ts.header.stamp = ros::Time::now();
    ts.header.frame_id = "world";
    int pos = (int)std::distance(msg->name.begin(), find(msg->name.begin(), msg->name.end(), name));
    ts.child_frame_id = name;
    ts.transform.translation.x = msg->pose.at(pos).position.x;
    ts.transform.translation.y = msg->pose.at(pos).position.y;
    ts.transform.rotation = msg->pose.at(pos).orientation;

    tfBroadcaster.sendTransform(ts);
}


void OnBotPose(const gazebo_msgs::ModelStates::ConstPtr& msg)
{
    SendTransformStamped("bot1", msg);
    SendTransformStamped("bot2", msg);

}

void OnLinkPose(const gazebo_msgs::LinkStates::ConstPtr& msg)
{
    SendTransformStamped("bot1::link_box3", msg);
    SendTransformStamped("bot2::link_box3", msg);
    SendTransformStamped("bot1::link_base", msg);
    SendTransformStamped("bot2::link_base", msg);
}


int main(int argc, char** argv)
{
    init(argc, argv, "bot2_ai_node");

    NodeHandle node;

    Subscriber subscriber1 = node.subscribe("/gazebo/model_states", 1000, &OnBotPose);
    Subscriber subscriber2 = node.subscribe("/gazebo/link_states", 1000, &OnLinkPose);

    Rate loop_rate(10);

    ROS_INFO("Bot tf publisher works!");

    spin();

    return 0;
}
