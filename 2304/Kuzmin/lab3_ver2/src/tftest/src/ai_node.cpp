#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include "tf/transform_datatypes.h"
#include <geometry_msgs/PointStamped.h>

double dX,dY,dA;
double linkX=5, linkY, linkA;
double baseX, baseY, baseA;
std::string bot_name, enemy_name;
double coef = 0.05;


int main(int argc, char** argv){
  ros::init(argc, argv, "ai_node");

  ros::NodeHandle node("~");
  node.getParam("bot_name", bot_name);
  node.getParam("enemy_name", enemy_name);

  ros::Publisher pose_pub = node.advertise<geometry_msgs::PointStamped>("/" + bot_name + "/body_pose", 10);

  tf2_ros::Buffer tfBuffer;
  tf2_ros::TransformListener tfListener(tfBuffer);

  ros::Rate rate(10.0);
  while (ros::ok()) {
      geometry_msgs::TransformStamped ts1;
      try {
          ts1 = tfBuffer.lookupTransform("world", enemy_name + "/body", ros::Time(0));
      }
      catch (tf::TransformException& ex) {
          ROS_ERROR("%s", ex.what());
          ros::Duration(1.0).sleep();
          continue;
      }

      tf::Quaternion tfQ;
      tf::quaternionMsgToTF(ts1.transform.rotation, tfQ);
      double temp;
      tf::Matrix3x3(tfQ).getRPY(temp, temp, baseA);

      baseX = ts1.transform.translation.x;
      baseY = ts1.transform.translation.y;

      dX = baseX - linkX+3*cos(baseA);
      dY = baseY - linkY+3*sin(baseA);
      dA = 3.14 + baseA - linkA;

      linkX+=coef*dX;
      linkY+=coef*dY;
      linkA+=coef*dA;

      //==================

      static tf2_ros::TransformBroadcaster br;
      geometry_msgs::TransformStamped ts2;

      ts2.header.stamp = ros::Time::now();
      ts2.header.frame_id = "world";
      ts2.child_frame_id = bot_name + "/body";
      ts2.transform.translation.x = linkX;
      ts2.transform.translation.y = linkY;

      geometry_msgs::Quaternion geoQ;
      tfQ = tf::Quaternion(0, 0, linkA);
      tf::quaternionTFToMsg(tfQ, geoQ);
      ts2.transform.rotation = geoQ;

      br.sendTransform(ts2);

      //==================

      geometry_msgs::PointStamped p;
      p.point.x = linkX;
      p.point.y = linkY;
      p.header.frame_id = "world";
      pose_pub.publish(p);

      ros::spinOnce();
      rate.sleep();
  }
  return 0;
};
