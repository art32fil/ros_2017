#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <turtlesim/Pose.h>
#include <fstream>

std::string name;

double c1 = 0.0;
double c2 = 0.0;
double c3 = 0.0;

double cd1 = 0.0;
double cd2 = 0.0;
double cd3 = 0.0;


void poseCallback(const turtlesim::PoseConstPtr& msg){
  static tf::TransformBroadcaster br;
  tf::Quaternion q;
  q.setRPY(0, 0,  c1);
  tf::Transform transform_bot1;
  transform_bot1.setOrigin( tf::Vector3(msg->x, msg->y, 0.0) );
  transform_bot1.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform_bot1, ros::Time::now(), "world", "Robot"+name));
  tf::Transform knee1;
  knee1.setOrigin( tf::Vector3(1.5, 0, 0.0) );
  q.setRPY(0, 0, -c2);
  knee1.setRotation(q);
  br.sendTransform(tf::StampedTransform(knee1, ros::Time::now(), "Robot"+name, name+"1"));
  tf::Transform knee2;
  knee2.setOrigin( tf::Vector3(0.5, 0, 0.0) );
  q.setRPY(0, 0, c3);
  knee2.setRotation(q);
  br.sendTransform(tf::StampedTransform(knee2, ros::Time::now(), name+"1", name+"2"));
  tf::Transform knee3;
  knee3.setOrigin( tf::Vector3(1.0, 0, 0.0) );
  q.setRPY(0, 0, 0);
  knee3.setRotation(q);
  br.sendTransform(tf::StampedTransform(knee3, ros::Time::now(), name+"2", name+"3"));


  c1+=cd1;
  c2+=cd2;
  c3+=cd3;

}

int main(int argc, char** argv){

  std::ifstream ifs("/dev/urandom", std::ifstream::in);
  std::srand(ifs.get());
  ifs.close();
  cd1 = 0.015 + (rand()%20)/1000.0;
  cd2 = 0.04 + (rand()%10)/1000.0;
  cd3 = 0.03 + (rand()%10)/1000.0;

  ros::init(argc, argv, "Robot");

  ros::NodeHandle node;
  ros::param::get("~name", name);
  ros::Subscriber sub = node.subscribe("pose", 10, &poseCallback);

  ros::spin();
  return 0;
};
