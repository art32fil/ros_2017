#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <turtlesim/Pose.h>
#include <fstream>
#include <geometry_msgs/PointStamped.h>
#include <visualization_msgs/Marker.h>



int main(int argc, char** argv){
ros::init(argc, argv, "restorator");
ros::NodeHandle node;
ros::Publisher chatter = node.advertise<visualization_msgs::Marker>("markers", 1);
tf::TransformListener listener;
ros::Rate rate(30.0);
while (node.ok()){
tf::StampedTransform transform;
try{
listener.lookupTransform("/BattlerB", "/A3",
 ros::Time(0), transform);
double x = transform.getOrigin().x();
double y = transform.getOrigin().y();
if(sqrt(x*x + y*y) <= 0.1){
ROS_WARN("Oxximiron won and it's success for us of course!");
ros::shutdown();
}
listener.lookupTransform("/BattlerA", "/B3",
 ros::Time(0), transform);
x = transform.getOrigin().x();
y = transform.getOrigin().y();
if(sqrt(x*x + y*y) <= 0.1){
ROS_ERROR("Gnoynii won and It's a lost of course!");
ros::shutdown();
}
listener.lookupTransform("/world", "/BattlerA",
 ros::Time(0), transform);
x = transform.getOrigin().x();
y = transform.getOrigin().y();
geometry_msgs::PointStamped p;
p.point.x = x; p.point.y = y; p.point.z = 0;
p.header.frame_id = "BattlerA";
visualization_msgs::Marker marker;
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
marker.scale.x = 1.0;
marker.scale.y = 1.0;
marker.scale.z = 1.0;
marker.header.stamp = ros::Time::now();
chatter.publish(marker);
geometry_msgs::PointStamped base_point;
listener.transformPoint("world", p, base_point);
}
catch (tf::TransformException ex){
}
rate.sleep();
}
return 0;
}
