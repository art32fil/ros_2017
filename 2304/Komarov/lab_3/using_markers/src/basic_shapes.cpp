#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "basic_shapes");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CYLINDER;

    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/my_frame";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "basic_shapes";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = 2;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 1.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.1;
    marker.scale.y = 0.5;
    marker.scale.z = 0.1;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    marker_pub.publish(marker);

    tf::TransformBroadcaster br;
    tf::Transform transform;
    tf::TransformListener listener;
    int i = 0;
  while (ros::ok())
  {
      transform.setOrigin( tf::Vector3(0.0, i, 0.0) );
      transform.setRotation( tf::Quaternion(1, 0, 0, 1) );
      br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "my_frame", "basic_shapes"));


      tf::StampedTransform stransform;
      try{
        listener.lookupTransform( "my_frame", "basic_shapes",
                                 ros::Time(0), stransform);
      }
      catch (tf::TransformException ex){
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
      }


      marker.pose.position.y = transform.getOrigin().y();
      marker.pose.orientation.x = stransform.getRotation().x();
      marker.pose.orientation.y = stransform.getRotation().y();
      marker.pose.orientation.z = stransform.getRotation().z();
      marker.pose.orientation.w = stransform.getRotation().w();
//      vel_msg.angular = 4.0 * atan2(transform.getOrigin().y(),
//                                  transform.getOrigin().x());
//      vel_msg.linear = 0.5 * sqrt(pow(transform.getOrigin().x(), 2) +
//                                  pow(transform.getOrigin().y(), 2));
      marker_pub.publish(marker);
      i++;
      r.sleep();
  }
}
