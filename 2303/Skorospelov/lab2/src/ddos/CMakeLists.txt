cmake_minimum_required(VERSION 2.8.3)
project(ddos)
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  genmsg
  message_generation
)

add_service_files(FILES msgs.srv)

generate_messages(DEPENDENCIES std_msgs)

catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(node src/node.cpp)
target_link_libraries(node ${catkin_LIBRARIES})
add_dependencies(node ddos_gencpp)
