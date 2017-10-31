cmake_minimum_required(VERSION 2.8.3)
project(maze)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_pub src/publisher.cpp)
add_executable(${PROJECT_NAME}_sub src/subscriber.cpp)

 target_link_libraries(${PROJECT_NAME}_pub
   ${catkin_LIBRARIES}
 )

 target_link_libraries(${PROJECT_NAME}_sub
   ${catkin_LIBRARIES}
 )
