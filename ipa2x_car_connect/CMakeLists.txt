# %Tag(FULLTEXT)%
cmake_minimum_required(VERSION 2.8.3)
project(ipa2x_car_connect)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)

# Include latest argparse
include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

## Declare a catkin package
catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})

add_subdirectory(idl)

add_library(common src/Common.cpp)
target_link_libraries(common argparse fastrtps fastcdr)

add_executable(dummy src/dummy.cpp)
target_link_libraries(dummy ${catkin_LIBRARIES})

add_executable(ipa2x_car_connect src/ipa2x_car_connect.cpp)
target_link_libraries(ipa2x_car_connect common idl fastrtps fastcdr argparse ${catkin_LIBRARIES})

# %EndTag(FULLTEXT)%
