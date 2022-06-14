cmake_minimum_required(VERSION 3.16)

project(iwr6843aop VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


include(FetchContent)
FetchContent_Declare(serial GIT_REPOSITORY https://github.com/kria-system/serial.git
   			GIT_TAG a1193a9b3b03389f7b79c9bdb6192c9d986fa87c
   			GIT_PROGRESS TRUE)
FetchContent_GetProperties(serial)
if(NOT serial_POPULATED)
    FetchContent_MakeAvailable(serial)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

# Export information to downstream packages
if(ROS_DISTRO IN_LIST ROS_DISTROS)
  ament_export_dependencies(ament_cmake rclcpp rosidl_default_runtime eigen3_cmake_module Eigen3 sensor_msgs)
else()
  ament_export_dependencies(ament_cmake rclcpp rosidl_default_runtime Eigen3 sensor_msgs)
endif()

add_subdirectory(src)
add_subdirectory(test)

# install the config directory
install(DIRECTORY
  cfg_files
  DESTINATION lib/${PROJECT_NAME}/
)

ament_package()
