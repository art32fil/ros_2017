#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include "tf/transform_datatypes.h"
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PointStamped.h>


int main(int argc, char** argv){
  ros::init(argc, argv, "enemy_node");
  ros::NodeHandle node("~");
  ros::Rate rate(10.0);
  ros::Publisher pose_pub = node.advertise<geometry_msgs::PointStamped>("/enemy/body_pose", 10);
	
  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);  
  while (ros::ok()) {
      geometry_msgs::TransformStamped ts1;
      try {
          ts1 = tfBuffer.lookupTransform("world", "player/body", ros::Time::now(), ros::Duration(3.0));
      }
      catch (tf::TransformException& ex) {
          ROS_ERROR("%s", ex.what());
          ros::Duration(1.0).sleep();
          continue;
      }
      tf::Quaternion tfQ;
      tf::quaternionMsgToTF(ts1.transform.rotation, tfQ);
      double temp, angl;
	  
      tf::Matrix3x3(tfQ).getRPY(temp, temp, angl); 
      static tf2_ros::TransformBroadcaster br;
      geometry_msgs::TransformStamped ts2;
      ts2.header.stamp = ros::Time::now();
      ts2.header.frame_id = "world";
      ts2.child_frame_id = "enemy/body";
      ts2.transform.translation.x = (ts1.transform.translation.x*0.15)+1.5;
      ts2.transform.translation.y = ts1.transform.translation.y*0.15;
      geometry_msgs::Quaternion geoQ;
      tfQ = tf::Quaternion(0, 0, angl-ts1.transform.translation.x);
      tf::quaternionTFToMsg(tfQ, geoQ);
      ts2.transform.rotation = geoQ;
      br.sendTransform(ts2);      

      geometry_msgs::PointStamped p;
      p.point.x = (ts1.transform.translation.x*0.15)+1.5;
      p.point.y = ts1.transform.translation.y*0.15;
      p.header.frame_id = "world";
      pose_pub.publish(p);

      ros::spinOnce();
      rate.sleep();
  }
  return 0;
};
