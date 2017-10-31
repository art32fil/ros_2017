cmake_minimum_required(VERSION 2.8.3)
project(treasurehunter)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp message_generation
)

add_message_files(
  FILES
  Pose.msg
)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(CATKIN_DEPENDS message_runtime)

include_directories(include ${catkin_INCLUDE_DIRS})
## link_directories(${catkin_LIBRARY_DIRS})

add_executable(maze_node src/maze.cpp)
target_link_libraries(maze_node ${catkin_LIBRARIES})

add_executable(hunter_node src/hunter.cpp)
target_link_libraries(hunter_node ${catkin_LIBRARIES})
