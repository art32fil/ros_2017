cmake_minimum_required(VERSION 2.8.3)
project(lab2_c)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs genmsg message_generation)

## Declare ROS messages and services
add_message_files(DIRECTORY msg
  FILES RobotControl.msg DriveUnitStatus.msg Coords.msg) 

add_service_files(DIRECTORY srv
  FILES MovingSrv.srv LocatingSrv.srv PickupingSrv.srv RepairingSrv.srv)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package(CATKIN_DEPENDS message_runtime)

include_directories(include ${catkin_INCLUDE_DIRS})

# Robot node
add_executable(robot src/robot.cpp)
target_link_libraries(robot ${catkin_LIBRARIES})
add_dependencies(robot ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Map service node
add_executable(map_service src/map_service.cpp)
target_link_libraries(map_service ${catkin_LIBRARIES})
add_dependencies(map_service ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Robot control node
add_executable(robot_control src/robot_control.cpp)
target_link_libraries(robot_control ${catkin_LIBRARIES})
add_dependencies(robot_control ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Robot's drive unit node
add_executable(drive_unit src/drive_unit.cpp)
target_link_libraries(drive_unit ${catkin_LIBRARIES})
add_dependencies(drive_unit ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Evil trickster node
add_executable(trickster src/trickster.cpp)
target_link_libraries(trickster ${catkin_LIBRARIES})
add_dependencies(trickster ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

# Repairing service node
add_executable(repair_service src/repair_service.cpp)
target_link_libraries(repair_service ${catkin_LIBRARIES})
add_dependencies(repair_service ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
