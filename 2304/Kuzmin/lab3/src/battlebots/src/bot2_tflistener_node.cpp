#include <vector>
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <gazebo_msgs/ModelStates.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>

using namespace ros;


int main(int argc, char** argv)
{
    init(argc, argv, "bot2_ai_node");

    NodeHandle node;

    Publisher publisher1 = node.advertise<geometry_msgs::Twist>("/sim/bot2/vel_cmd", 10);
    Rate loop_rate(10);

    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener tfListener(tfBuffer);

    ROS_INFO("Bot2 tf listener works!");

    while (ok()) {

        geometry_msgs::TransformStamped ts2;
            try{
              ts2 = tfBuffer.lookupTransform("bot2", "bot1", ros::Time(0));
            }
            catch (tf2::TransformException &ex) {
              ROS_WARN("%s",ex.what());
              ros::Duration(1.0).sleep();
              continue;
            }

            geometry_msgs::Twist vel_msg;

            //=0-4*atan2(deltaX, deltaY)
            vel_msg.angular.z = -4.0 * atan2(ts2.transform.translation.y, ts2.transform.translation.x);

            //=1+0.2*sqrt(deltaX^2+deltay^2)
            vel_msg.linear.x = 1+0.2 * sqrt(pow(ts2.transform.translation.x, 2) + pow(ts2.transform.translation.y, 2));

            publisher1.publish(vel_msg);

        loop_rate.sleep();
    }

    return 0;
}
