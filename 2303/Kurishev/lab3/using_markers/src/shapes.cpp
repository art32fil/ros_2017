
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_broadcaster.h>
#include <iostream>

class Robot{
  public:
  void callback(const geometry_msgs::Twist::ConstPtr& vel){

    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.frame_id = "/odom";
    odom_trans.child_frame_id = "/my_frame";

 const double degree = M_PI/180;

   
        odom_trans.header.stamp = ros::Time::now();
        odom_trans.transform.translation.x = cos(angle)*2;
        odom_trans.transform.translation.y = sin(angle)*2;
        odom_trans.transform.translation.z = .7;
        odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(angle+M_PI/2);
           angle += degree/4;
 
    br.sendTransform(odom_trans);
    
    //std::cout<<"vel = "<< *vel; 
    marker.pose.position.x++;
   }
    public:
    Robot(){
    angle = 0;
      pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);//mb add to Marker word Array 
      sub = n.subscribe("/RobotControl", 10,&Robot::callback, this);
      // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    bodyShape = visualization_msgs::Marker::CYLINDER;
    shape = visualization_msgs::Marker::CUBE;
    marker.header.frame_id = "/my_frame";
    body.header.frame_id = "/odom";
    marker.header.stamp = ros::Time::now();
    body.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "basic_shapes";
    marker.id = 0;
    body.ns = "basic_shapes";
    body.id = 1;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;
    body.type = bodyShape;
    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;
    body.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    body.pose.position.x = 5;
    body.pose.position.y = 5;
    body.pose.position.z = 0;
    body.pose.orientation.x = 0.0;
    body.pose.orientation.y = 0.0;
    body.pose.orientation.z = 0.0;
    body.pose.orientation.w = 3.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 1.0;
    marker.scale.y = 1.0;
    marker.scale.z = 1.0;

    body.scale.x = 1.0;
    body.scale.y = 1.0;
    body.scale.z = 1.0;
    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 1.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    body.color.r = 0.0f;
    body.color.g = 0.0f;
    body.color.b = 1.0f;
    body.color.a = 1.0;

    body.lifetime = ros::Duration();
  
    }
    // Set our initial shape type to be a cubed
    uint32_t shape;
    uint32_t bodyShape;
    visualization_msgs::Marker marker;
    visualization_msgs::Marker body;
    ros::NodeHandle n;
    ros::Publisher pub;
    ros::Subscriber sub;

    tf::TransformBroadcaster br;
    tf::Transform transform;

  double angle;

};


int main( int argc, char** argv )
{
  ros::init(argc, argv, "RobotsBattle");
  Robot robot1;
     
    ros::Rate loop_rate(5);
 while (ros::ok()) {

     

   robot1.pub.publish(robot1.marker);
    robot1.pub.publish(robot1.body);
   
   loop_rate.sleep();
  ros::spinOnce();
 }
  return 0;
}



/*
function CLion {  ~/Downloads/clion-2017.2.2/bin/clion.sh }
*/
