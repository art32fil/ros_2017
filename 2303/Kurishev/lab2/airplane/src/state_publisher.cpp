#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include "std_msgs/String.h"

class StatePublisher
{
public:
  StatePublisher()
  {
    //Topic you want to publish
    joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);
    //Topic you want to subscribe
    sub = n.subscribe("/destroyed", 1000,&StatePublisher::callback, this);
  }

  void callback(const std_msgs::String::ConstPtr& msg)
  {
    ROS_INFO("more then 2 engine was destroyed, airplane was destroyed");
    if (msg->data == "kill")
	ros::shutdown();
  }

  int count_destroyed;
  char str_temp[1];
  std_msgs::String msg;
  ros::NodeHandle n; 
  ros::Publisher joint_pub;
  ros::Subscriber sub;
  tf::TransformBroadcaster broadcaster;

};//End of class SubscribeAndPublish

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "state_publisher");

  //Create an object of class SubscribeAndPublish that will take care of everything
  StatePublisher StPubObject;

  ros::Rate loop_rate(30);
   const double degree = M_PI/180;

    // robot state
    double tilt = 0, tinc = degree, swivel=0, angle=0, height=0, hinc=0.005;

    // message declarations
    geometry_msgs::TransformStamped odom_trans;
    sensor_msgs::JointState joint_state;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_body";

      while (ros::ok()) {
        //update joint_state
        joint_state.header.stamp = ros::Time::now();
        joint_state.name.resize(8);
	joint_state.position.resize(8);
     /*
        joint_state.name[0] ="swivel";
        joint_state.position[0] = swivel;
        joint_state.name[1] ="tilt";
        joint_state.position[1] = tilt;
        joint_state.name[2] ="periscope";
        joint_state.position[2] = height;
    */


        // update transform
        // (moving in a circle with radius=2)
        odom_trans.header.stamp = ros::Time::now();
        odom_trans.transform.translation.x = cos(angle)*2;
        odom_trans.transform.translation.y = sin(angle)*2;
        odom_trans.transform.translation.z = .7;
        odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(angle+M_PI/2);

        //send the joint state and transform
        StPubObject.joint_pub.publish(joint_state);
        StPubObject.broadcaster.sendTransform(odom_trans);

        // Create new robot state
        /*tilt += tinc;
        if (tilt<-.5 || tilt>0) tinc *= -1;
        height += hinc;
        if (height>.2 || height<0) hinc *= -1;
        swivel += degree;
         */
        angle += degree/4;

        // This will adjust as needed per iteration
        loop_rate.sleep();
      ros::spinOnce();
    }

  return 0;
}
