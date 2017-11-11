#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <turtlesim/Pose.h>
#include <fstream>

std::string name;

double coefficient1 = 0.0;
double coefficient2 = 0.0;
double coefficient3 = 0.0;

double newValue1 = 0.0;
double newValue2 = 0.0;
double newValue3 = 0.0;


void poseCallback(const turtlesim::PoseConstPtr& msg){
  static tf::TransformBroadcaster br;
  tf::Quaternion q;
  q.setRPY(0, 0,  coefficient1);
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(msg->x, msg->y, 0.0) );
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "Player"+name));
  if(name == "A")
  {
      tf::Transform knee1;
      knee1.setOrigin( tf::Vector3(1.5, 0, 0.0) );
      q.setRPY(0, 0, -coefficient2);
      knee1.setRotation(q);
      br.sendTransform(tf::StampedTransform(knee1, ros::Time::now(), "Player"+name, name+"1"));
      tf::Transform knee2;
      knee2.setOrigin( tf::Vector3(0.5, 0, 0.0) );
      q.setRPY(0, 0, coefficient3);
      knee2.setRotation(q);
      br.sendTransform(tf::StampedTransform(knee2, ros::Time::now(), name+"1", name+"2"));
  }
  else
  {
      tf::Transform knee1;
      knee1.setOrigin( tf::Vector3(1.5, 0, 0.0) );
      q.setRPY(0, 0, coefficient2);
      knee1.setRotation(q);
      br.sendTransform(tf::StampedTransform(knee1, ros::Time::now(), "Player"+name, name+"1"));
      tf::Transform knee2;
      knee2.setOrigin( tf::Vector3(0.5, 0, 0.0) );
      q.setRPY(0, 0, -coefficient3);
      knee2.setRotation(q);
      br.sendTransform(tf::StampedTransform(knee2, ros::Time::now(), name+"1", name+"2"));
  }
  tf::Transform knee3;
  knee3.setOrigin( tf::Vector3(1.0, 0, 0.0) );
  q.setRPY(0, 0, 0);
  knee3.setRotation(q);
  br.sendTransform(tf::StampedTransform(knee3, ros::Time::now(), name+"2", name+"3"));

  coefficient1+=newValue1;
  coefficient2+=newValue2;
  coefficient3+=newValue3;
}

int main(int argc, char** argv){
  newValue1 = 0.015 + (rand()%20)/1000.0;
  newValue2 = 0.04 + (rand()%10)/1000.0;
  newValue3 = 0.03 + (rand()%10)/1000.0;

  ros::init(argc, argv, "Player");

  ros::NodeHandle node;
  ros::param::get("~name", name);
  ros::Subscriber sub = node.subscribe("pose", 10, &poseCallback);

  ros::spin();
  return 0;
};
