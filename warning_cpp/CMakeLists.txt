cmake_minimum_required(VERSION 3.12.4)

if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

project(warning_cpp)

# Find requirements
if(NOT fastcdr_FOUND)
    find_package(fastcdr REQUIRED)
endif()

if(NOT fastrtps_FOUND)
    find_package(fastrtps REQUIRED)
endif()

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

message(STATUS "Configuring CrossingInfo and CarInfo publisher/subscriber and Discovery server...")
file(GLOB SOURCES_CXX "src/include/*.cxx")

add_executable(InfoPub src/CarInfoPublisher.cpp ${SOURCES_CXX})
target_link_libraries(InfoPub fastrtps fastcdr argparse)

add_executable(InfoSub src/CarInfoSubscriber.cpp ${SOURCES_CXX})
target_link_libraries(InfoSub fastrtps fastcdr argparse)

add_executable(CrossPub src/CrossingPublisher.cpp ${SOURCES_CXX})
target_link_libraries(CrossPub fastrtps fastcdr argparse)

add_executable(CrossSub src/CrossingSubscriber.cpp ${SOURCES_CXX})
target_link_libraries(CrossSub fastrtps fastcdr argparse)

add_executable(DiscoveryServer src/DiscoveryServer.cpp ${SOURCES_CXX})
target_link_libraries(DiscoveryServer fastrtps fastcdr argparse)
