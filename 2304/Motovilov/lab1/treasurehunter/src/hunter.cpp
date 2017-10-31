#include <ros/ros.h>
#include <treasurehunter/Pose.h>
#include "getch.cpp"

const int KEYCODE_UP = 65;
const int KEYCODE_RIGHT = 67;
const int KEYCODE_DOWN = 66;
const int KEYCODE_LEFT = 68;

int main(int argc, char** argv)
{
  ros::init(argc, argv, "hunter_publisher");
  ros::NodeHandle n;
  ros::Publisher pub_moveHunter = n.advertise<treasurehunter::Pose>("moveHunter_topic", 1000);

  ros::Rate loop_rate(10);
    
  puts("Reading from keyboard");
  puts("---------------------------");
  puts("Use arrow keys to move the turtle.");

  int key;

  while (ros::ok() && ((key = getch()))) {
    treasurehunter::Pose msg;
    
    switch (key) {
      case KEYCODE_UP:
          ROS_INFO("Up");
          msg.key = 0;
          break;
      case KEYCODE_RIGHT:
          ROS_INFO("Right");
          msg.key = 1;
          break;
      case KEYCODE_DOWN:
          ROS_INFO("Down");
          msg.key = 2;
          break;
      case KEYCODE_LEFT:
          ROS_INFO("Left");
          msg.key = 3;
          break;   
      default:
          msg.key = -1;
          break;
    }

    pub_moveHunter.publish(msg);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}