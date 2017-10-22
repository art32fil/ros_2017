cmake_minimum_required(VERSION 2.8.3)
project(handyman)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
)

add_service_files(
  FILES
  AddTwoInts.srv
  Repair.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

## Declare a catkin package
catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

## Build talker and listener
add_executable(master src/master.cpp)
target_link_libraries(master ${catkin_LIBRARIES})
add_dependencies(master handyman_gencpp)

add_executable(plainSystem src/plainSystem.cpp)
target_link_libraries(plainSystem ${catkin_LIBRARIES})
add_dependencies(plainSystem handyman_gencpp)
