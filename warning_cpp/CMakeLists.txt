cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(ipa2x-warning)

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

# Set C++17
include(CheckCXXCompilerFlag)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    check_cxx_compiler_flag(-std=c++17 SUPPORTS_CXX17)
    if(SUPPORTS_CXX17)
        add_compile_options(-std=c++17)
    else()
        message(FATAL_ERROR "Compiler doesn't support C++17")
    endif()
endif()

add_subdirectory(idl)

add_executable(InfoPub src/CarInfoPublisher.cpp)
target_link_libraries(InfoPub idl fastrtps fastcdr argparse)

add_executable(InfoSub src/CarInfoSubscriber.cpp)
target_link_libraries(InfoSub idl fastrtps fastcdr argparse)

add_executable(CrossPub src/CrossingPublisher.cpp)
target_link_libraries(CrossPub idl fastrtps fastcdr argparse)

add_executable(CrossSub src/CrossingSubscriber.cpp)
target_link_libraries(CrossSub idl fastrtps fastcdr argparse)

add_executable(DiscoveryServer src/DiscoveryServer.cpp)
target_link_libraries(DiscoveryServer idl fastrtps fastcdr argparse)
