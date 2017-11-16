#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include "tf/transform_datatypes.h"
#include <geometry_msgs/PointStamped.h>


double dX,dY,dA;
double linkX, linkY, linkA;
double baseX, baseY, baseA;

std::string link_name, prev_link_name, bot_name;

double coef = 0.1;

int main(int argc, char** argv)
{
    ros::init(argc, argv, "link_node");

    ros::NodeHandle node("~");
    node.getParam("link_name", link_name);
    node.getParam("prev_link_name", prev_link_name);
    node.getParam("bot_name", bot_name);

    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener tfListener(tfBuffer);

    ros::Rate rate(10.0);
    while (ros::ok()) {
        geometry_msgs::TransformStamped ts1;
        try {
            ts1 = tfBuffer.lookupTransform("world", bot_name + "/" + prev_link_name, ros::Time(0));
        }
        catch (tf::TransformException& ex) {
            ROS_ERROR("%s", ex.what());
            ros::Duration(1.0).sleep();
            continue;
        }

        //=======

        tf::Quaternion tfQ;
        tf::quaternionMsgToTF(ts1.transform.rotation, tfQ);
        double temp;
        tf::Matrix3x3(tfQ).getRPY(temp, temp, baseA);

        baseX = ts1.transform.translation.x;
        baseY = ts1.transform.translation.y;

        dX = baseX - linkX;
        dY = baseY - linkY;
        dA = baseA - linkA;

        linkX+=coef*dX;
        linkY+=coef*dY;
        linkA+=coef*dA;

        //=======

        static tf2_ros::TransformBroadcaster br;
        geometry_msgs::TransformStamped ts2;

        ts2.header.stamp = ros::Time::now();
        ts2.header.frame_id = bot_name + "/" + prev_link_name;
        ts2.child_frame_id = bot_name + "/" + link_name;
        ts2.transform.translation.x = (dX + 0.5)*cos(dA);
        ts2.transform.translation.y = dY*sin(dA);

        geometry_msgs::Quaternion geoQ;
        tfQ = tf::Quaternion(0, 0, dA);
        tf::quaternionTFToMsg(tfQ, geoQ);
        ts2.transform.rotation = geoQ;

        br.sendTransform(ts2);

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
};
