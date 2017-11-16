#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include "tf/transform_datatypes.h"
#include <geometry_msgs/PointStamped.h>
#include <tf/LinearMath/Matrix3x3.h>

double x = 0, y = 0, yaw = 0;
std::string bot_name;

void on_vel(const geometry_msgs::Twist& msg)
{
    x += 0.05 * msg.linear.x * cos(yaw);
    y += 0.05 * msg.linear.x * sin(yaw);
    yaw += 0.05 * msg.angular.z;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "bot_node");

    ros::NodeHandle node("~");
    node.getParam("bot_name", bot_name);

    ros::Subscriber vel_sub = node.subscribe("/" + bot_name + "/cmd_vel", 1000, on_vel);
    ros::Publisher pose_pub = node.advertise<geometry_msgs::PointStamped>("/" + bot_name + "/body_pose", 10);

    ros::Rate rate(10.0);
    while (ros::ok()) {

        static tf2_ros::TransformBroadcaster br;
        geometry_msgs::TransformStamped ts;

        ts.header.stamp = ros::Time::now();
        ts.header.frame_id = "world";
        ts.child_frame_id = bot_name + "/body";
        ts.transform.translation.x = x;
        ts.transform.translation.y = y;

        geometry_msgs::Quaternion geoQ;
        tf::Quaternion tfQ(0, 0, yaw);
        tf::quaternionTFToMsg(tfQ, geoQ);
        ts.transform.rotation = geoQ;

        br.sendTransform(ts);

        //==================

        geometry_msgs::PointStamped p;
        p.point.x = x;
        p.point.y = y;
        p.header.frame_id = "world";
        pose_pub.publish(p);

        ros::spinOnce();
        rate.sleep();
    }
    return 0;
};
