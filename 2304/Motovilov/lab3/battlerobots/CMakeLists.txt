cmake_minimum_required(VERSION 2.8.3)
project(battlerobots)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  std_srvs
  tf
  turtlesim
)

find_package(Curses REQUIRED)

## Declare a catkin package
catkin_package()

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${CURSES_INCLUDE_DIR}
)

## Build talker and listener
add_executable(teleop src/teleop.cpp)
target_link_libraries(teleop ${catkin_LIBRARIES})
target_link_libraries(teleop  ${CURSES_LIBRARIES})

add_executable(arena src/arena.cpp)
target_link_libraries(arena ${catkin_LIBRARIES})

add_executable(bot src/bot.cpp)
target_link_libraries(bot ${catkin_LIBRARIES})

##target_link_libraries(basic_shapes ${catkin_LIBRARIES})
##add_executable(basic_shapes src/basic_shapes.cpp)
##target_link_libraries(basic_shapes ${catkin_LIBRARIES})
