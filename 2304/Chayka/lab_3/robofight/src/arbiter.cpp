#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <turtlesim/Pose.h>
#include <fstream>
#include <geometry_msgs/PointStamped.h>
#include <visualization_msgs/Marker.h>



int main(int argc, char** argv){
  ros::init(argc, argv, "arbiter");

  ros::NodeHandle node;
  ros::Publisher chatter = node.advertise<visualization_msgs::Marker>("markers", 1);

  tf::TransformListener listener;

  ros::Rate rate(30.0);
  while (node.ok()){
    tf::StampedTransform transform;
    try{
      listener.lookupTransform("/RobotB", "/A3",
                               ros::Time(0), transform);
      double x = transform.getOrigin().x();
      double y = transform.getOrigin().y();
      if(sqrt(x*x + y*y) <= 0.1){
        ROS_WARN("You win!!!");
        ros::shutdown();
      }

      listener.lookupTransform("/RobotA", "/B3",
                               ros::Time(0), transform);
      x = transform.getOrigin().x();
      y = transform.getOrigin().y();
      if(sqrt(x*x + y*y) <= 0.1){
        ROS_ERROR("You LOOOOOOOSE!!!");
        ros::shutdown();
      }

      listener.lookupTransform("/world", "/RobotA",
                               ros::Time(0), transform);
      x = transform.getOrigin().x();
      y = transform.getOrigin().y();

      geometry_msgs::PointStamped p;
      p.point.x = x; p.point.y = y; p.point.z = 0;
      p.header.frame_id = "RobotA";

      visualization_msgs::Marker marker;
      //marker.type = marker.LINE_STRIP;
      marker.action = visualization_msgs::Marker::ADD;
      marker.ns = "world";
      marker.id = 0;
      marker.points.push_back(p.point);
      p.point.x+=1;
      marker.points.push_back(p.point);
      marker.header.frame_id = "/world";
    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 1.0;
    marker.scale.y = 1.0;
    marker.scale.z = 1.0;
      marker.header.stamp = ros::Time::now();
      chatter.publish(marker);

      geometry_msgs::PointStamped base_point;
      listener.transformPoint("world", p, base_point);
//      ROS_INFO(">> (%lf %lf) --> (%lf %lf)", p.point.x, p.point.y, base_point.point.x, base_point.point.y);
      
    }
    catch (tf::TransformException ex){
    }
    rate.sleep();
  }

  return 0;
}
