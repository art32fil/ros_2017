#include <vector>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <tf2_ros/transform_broadcaster.h>
#include <gazebo_msgs/ModelStates.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/TransformStamped.h>

using namespace ros;

void OnBot1Pose(const gazebo_msgs::ModelStates::ConstPtr& msg)
{
    static tf2_ros::TransformBroadcaster tfBroadcaster;
    geometry_msgs::TransformStamped ts;
    ts.header.stamp = ros::Time::now();
    ts.header.frame_id = "world";

    //tf2::Quaternion q;
    //q.setRPY(0, 0, msg->pose.at(pos).orientation); //look at bot1
    //ts.transform.rotation.y = q.y();
    //ts.transform.rotation.z = q.z();
    //ts.transform.rotation.w = q.w();

    int pos = (int)std::distance(msg->name.begin(), find(msg->name.begin(), msg->name.end(), "bot1"));
    ts.child_frame_id = "bot1";
    ts.transform.translation.x = msg->pose.at(pos).position.x;
    ts.transform.translation.y = msg->pose.at(pos).position.y;
    ts.transform.rotation = msg->pose.at(pos).orientation;
    tfBroadcaster.sendTransform(ts);

    pos = (int)std::distance(msg->name.begin(), find(msg->name.begin(), msg->name.end(), "bot2"));
    ts.child_frame_id = "bot2";
    ts.transform.translation.x = msg->pose.at(pos).position.x;
    ts.transform.translation.y = msg->pose.at(pos).position.y;
    ts.transform.rotation = msg->pose.at(pos).orientation;
    tfBroadcaster.sendTransform(ts);

}


int main(int argc, char** argv)
{
    init(argc, argv, "bot2_ai_node");

    NodeHandle node;

    Subscriber subscriber1 = node.subscribe("/gazebo/model_states", 1000, &OnBot1Pose);

    Rate loop_rate(10);

    ROS_INFO("Bot tf publisher works!");

    spin();

    return 0;
}
