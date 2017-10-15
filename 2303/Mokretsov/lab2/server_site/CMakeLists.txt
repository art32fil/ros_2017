# %Tag(FULLTEXT)%
cmake_minimum_required(VERSION 2.8.3)
project(server_site)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg)

## Declare ROS messages and services
#add_message_files(FILES page_request.msg)
#add_service_files(FILES server_response.srv)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(page_index src/page_index.cpp)
target_link_libraries(page_index ${catkin_LIBRARIES})
add_dependencies(page_index server_site_gencpp)

add_executable(page_blog src/page_blog.cpp)
target_link_libraries(page_blog ${catkin_LIBRARIES})
add_dependencies(page_blog server_site_gencpp)

add_executable(page_login src/page_login.cpp)
target_link_libraries(page_login ${catkin_LIBRARIES})
add_dependencies(page_login server_site_gencpp)

add_executable(server src/server.cpp)
target_link_libraries(server ${catkin_LIBRARIES})
add_dependencies(server server_site_gencpp)
# %EndTag(FULLTEXT)%

