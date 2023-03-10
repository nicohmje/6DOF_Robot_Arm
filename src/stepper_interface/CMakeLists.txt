cmake_minimum_required(VERSION 3.8)
project(stepper_interface)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -fPIC)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(serial REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(controller_manager REQUIRED)
find_package(rclcpp REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)


add_library(${PROJECT_NAME} SHARED src/stepper_interface.cpp src/arduino_comm.cpp)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
  include
)
ament_target_dependencies(
  ${PROJECT_NAME}
  hardware_interface
  controller_manager
  serial
  rclcpp
  rclcpp_lifecycle
  pluginlib
)

target_compile_definitions(${PROJECT_NAME} PRIVATE "STEPPER_INTERFACE_BUILDING_DLL")


pluginlib_export_plugin_description_file(hardware_interface stepper_interface.xml)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION lib
)
install(
  DIRECTORY include/
  DESTINATION include
)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
endif()


## EXPORTS
ament_export_include_directories(
  include
)
ament_export_libraries(
  ${PROJECT_NAME}
)
ament_export_dependencies(
  hardware_interface
  pluginlib
  serial
  controller_manager
  rclcpp
  rclcpp_lifecycle
)
ament_package()

#WIP 
