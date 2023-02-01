cmake_minimum_required(VERSION 3.8)
project(stepper_interface)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(serial REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(controller_manager REQUIRED)
find_package(rclcpp REQUIRED)
find_package(pluginlib REQUIRED)

add_library(stepper_interface SHARED src/stepper_interface.cpp src/arduino_comm.cpp)

target_include_directories(
  stepper_interface
  PRIVATE
  include
)
ament_target_dependencies(
  stepper_interface
  hardware_interface
  controller_manager
  serial
  rclcpp
  pluginlib
)

pluginlib_export_plugin_description_file(hardware_interface stepper_interface.xml)


add_library(fake_robot SHARED src/fake_robot.cpp src/wheel.cpp)
set_property(TARGET fake_robot PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(
  fake_robot
  PRIVATE
  include
)
ament_target_dependencies(
  fake_robot
  hardware_interface
  controller_manager
  rclcpp
  pluginlib
)

pluginlib_export_plugin_description_file(hardware_interface fake_robot_hardware.xml)




install(
  TARGETS stepper_interface
  DESTINATION lib
)

ament_export_libraries(
  stepper_interface
)

ament_export_dependencies(
  hardware_interface
  controller_manager
  serial
  rclcpp
  pluginlib
)

ament_package()

#WIP 

#Written for ROS2 Foxy, maybe not compatible with Humble distro