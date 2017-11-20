
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <iostream>

class Robot {
public:
    void callback(const geometry_msgs::Twist::ConstPtr &vel) {
        if (vel->linear.x == 2) {
            x--;
        }
        if (vel->linear.x == -2) {
            x++;
        }
        if (vel->angular.z == -2) {
            y++;
        }
        if (vel->angular.z == 2) {
            y--;
        }
    }
    
public:
    Robot(std::string robotName, bool r, std::string topic) {
        
        name = robotName;
        x = 0;
        y = 0;

        pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);//mb add to Marker word Array 
        sub = n.subscribe(topic, 10, &Robot::callback, this);

        core = cloneMarker(robotName, r);
        knee1 = cloneMarker(robotName + "knee1", r);
        knee2 = cloneMarker(robotName + "knee2", r);
    }

    std::string getName() {
        return name;
    }

    visualization_msgs::Marker cloneMarker(std::string frame_id, bool r) {

        visualization_msgs::Marker marker;
        marker.header.frame_id = frame_id;
        marker.header.stamp = ros::Time(0);//ros::Time::now();
        
        // Set the namespace and id for this marker.  This serves to create a unique ID
        // Any marker sent with the same namespace and id will overwrite the old one
        marker.ns = "robots";
        marker.id = count;
        count++;
        
        // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
        // marker.type = shape;
        marker.type = visualization_msgs::Marker::SPHERE;
        marker.action = visualization_msgs::Marker::ADD;
        
        // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
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
        
        // Set the color -- be sure to set alpha to something non-zero!
        marker.color.r = 1.0f;
        marker.color.g = 0.0f;
        marker.color.b = 0.0f;
        marker.color.a = 1.0;

        if (r == false) {
            marker.color.r = 0.0f;
            marker.color.b = 1.0f;
        }

        marker.lifetime = ros::Duration();

        return marker;

    }

    // Set our initial shape type to be a cubed
    std::string name;

    visualization_msgs::Marker core;
    visualization_msgs::Marker knee1;
    visualization_msgs::Marker knee2;

    ros::NodeHandle n;
    ros::Publisher pub;
    ros::Subscriber sub;
    
    double x;
    double y;

    static int count;

};

int Robot::count = 0;

int main(int argc, char **argv) {
    
    ros::init(argc, argv, "RobotsBattle");

    bool redColor = true;
    Robot robot1 = Robot("robot1", redColor, "/RobotControl");
    robot1.x = 11;
    Robot robot2 = Robot("robot2", !redColor, "/RobotControl2");
    
    ros::Rate loop_rate(30);
    
    const double degree = M_PI / 180;
    double angle = 0;
    
    static tf::TransformBroadcaster br;
    tf::Transform transform;
    tf::StampedTransform transform1;
    tf::TransformListener listener;
    tf::Quaternion q;
    
    std::string robot1Name = robot1.getName();
    std::string robot2Name = robot2.getName();

    while (ros::ok()) {
        
        transform.setOrigin(tf::Vector3(robot1.x, robot1.y, 0));
        q.setRPY(0, 0, 0);
        transform.setRotation(q);
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", robot1Name));


        transform.setOrigin(tf::Vector3(cos(angle * 8) * 3, sin(angle * 8) * 3, 0));
        q.setRPY(0, 0, 0);
        transform.setRotation(q);
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), robot1Name, robot1Name + "knee1"));


        transform.setOrigin(tf::Vector3(cos(angle * 16) * 2, sin(angle * 16) * 2, 0));
        q.setRPY(0, 0, 0);
        transform.setRotation(q);
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), robot1Name + "knee1", robot1Name + "knee2"));

//----------------------
        transform.setOrigin(tf::Vector3(robot2.x, robot2.y, 0));

        q.setRPY(0, 0, 0);
        transform.setRotation(q);
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", robot2Name));


        transform.setOrigin(tf::Vector3(cos(angle * 16) * 6, sin(angle * 16) * 6, 0));
        q.setRPY(0, 0, 0);
        transform.setRotation(q);
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), robot2Name, robot2Name + "knee1"));


        transform.setOrigin(tf::Vector3(cos(angle * 32) * 4, sin(angle * 32) * 4, 0));
        q.setRPY(0, 0, 0);
        transform.setRotation(q);
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), robot2Name + "knee1", robot2Name + "knee2"));
        
        robot1.pub.publish(robot1.core);
        robot1.pub.publish(robot1.knee1);
        robot1.pub.publish(robot1.knee2);

        robot2.pub.publish(robot2.core);
        robot2.pub.publish(robot2.knee1);
        robot2.pub.publish(robot2.knee2);
        
        try {
            listener.lookupTransform("robot1", "robot2knee2",
                                     ros::Time(0), transform1);
            double x = transform1.getOrigin().x();
            double y = transform1.getOrigin().y();
            if (sqrt(x * x + y * y) <= 0.8) {
                ROS_WARN("Your Robot died!");
                ros::shutdown();
            }
            
            listener.lookupTransform("robot1", "robot2knee1",
                                     ros::Time(0), transform1);
            x = transform1.getOrigin().x();
            y = transform1.getOrigin().y();
            if (sqrt(x * x + y * y) <= 0.8) {
                ROS_WARN("Your Robot died!");
                ros::shutdown();
            }
            
            listener.lookupTransform("robot1", "robot2",
                                     ros::Time(0), transform1);
            x = transform1.getOrigin().x();
            y = transform1.getOrigin().y();
            if (sqrt(x * x + y * y) <= 0.8) {
                ROS_WARN("Your Robot died!");
                ros::shutdown();
            }

            listener.lookupTransform("robot2", "robot1knee2",
                                     ros::Time(0), transform1);
            x = transform1.getOrigin().x();
            y = transform1.getOrigin().y();
            if (sqrt(x * x + y * y) <= 0.8) {
                ROS_WARN("Your Robot won!");
                ros::shutdown();
            }
        }
        catch (tf::TransformException ex) {};
        
        angle += degree / 4;

        loop_rate.sleep();
        ros::spinOnce();
    }
    return 0;
}

