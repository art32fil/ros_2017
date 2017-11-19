#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include "tf/transform_datatypes.h"
#include <tf/LinearMath/Matrix3x3.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PointStamped.h>


double _xCoordinate = 0, _yCoordinate = 0, _theta = 0;

void CallBack(const geometry_msgs::Twist& msg){	
    _xCoordinate += 0.05 * msg.linear.x * cos(_theta);
    _yCoordinate += 0.05 * msg.linear.x * sin(_theta);
    _theta += 0.05 * msg.angular.z;
}

void SendTransform(){
		static tf2_ros::TransformBroadcaster br;
        geometry_msgs::TransformStamped transStam;        
        transStam.header.frame_id = "world";
        transStam.child_frame_id = "player/body";
		transStam.header.stamp = ros::Time::now();
        transStam.transform.translation.x = _xCoordinate;
        transStam.transform.translation.y = _yCoordinate;
        geometry_msgs::Quaternion quat;
        tf::Quaternion tfQ(0, 0, _theta);
        tf::quaternionTFToMsg(tfQ, quat);
        transStam.transform.rotation = quat;

        br.sendTransform(transStam);
}

geometry_msgs::PointStamped Point(){
		geometry_msgs::PointStamped ps;
        ps.point.x = _xCoordinate;
        ps.point.y = _yCoordinate;
        ps.header.frame_id = "world";
	return ps;
}

int main(int argc, char** argv){
    ros::init(argc, argv, "player_node");
    ros::NodeHandle node("~");	    
    ros::Subscriber vel_sub = node.subscribe("/player/cmd_vel", 1000, CallBack);
    ros::Publisher pose_pub = node.advertise<geometry_msgs::PointStamped>("/player/body_pose", 10);
    ros::Rate rate(10.0);
    while (ros::ok()) {    
		SendTransform();        
        pose_pub.publish(Point());
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
};
