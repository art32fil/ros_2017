#include "ros/ros.h"
#include "bicycle/Repair.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "repair_client");

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<bicycle::Repair>("repair");
  bicycle::Repair srv;

  int countRepairedDetails = 0;
  ROS_INFO("Ready");
  while (countRepairedDetails < 10) {
      int detailCode;
      std::cin >> detailCode;
      if(detailCode >=0 && detailCode <=4){
          srv.request.repairCode = detailCode;

          if (client.call(srv)) {
              if (srv.response.isSuccess == true){
                  countRepairedDetails = srv.response.countRepairedDetails;
                  ROS_INFO("Detail %s repaired", srv.response.detail.c_str());
              }
              else
                  ROS_INFO("Error, bad request");
          }
          else {
              ROS_ERROR("ERROR");
              return 1;
          }
      }
      else
          ROS_INFO("Error, not found detail");
      ros::spinOnce();
}

  return 0;
}
