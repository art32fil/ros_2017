# %Tag(FULLTEXT)%
cmake_minimum_required(VERSION 2.8.3)
project(server_site)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg message_generation)

## Declare ROS messages and services
add_message_files(FILES Page.msg)
add_service_files(FILES SrvMessage.srv)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(client src/client.cpp)
target_link_libraries(client ${catkin_LIBRARIES})
add_dependencies(client server_site_gencpp)

add_executable(server src/server.cpp)
target_link_libraries(server ${catkin_LIBRARIES})
add_dependencies(server server_site_gencpp)
# %EndTag(FULLTEXT)%

