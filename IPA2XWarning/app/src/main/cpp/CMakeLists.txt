# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
set(DDS_PATH D:/FEL/PRJ/DDS/Fast-DDSw/install/)
cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("ipa2xwarning")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        ipa2xwarning

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        crossing/CrossingInfo.cxx
        crossing/Gps.cxx
        crossing/GpsPubSubTypes.cxx
        crossing/CrossingInfoPubSubTypes.cxx
        crossing/CrossingSubscriber.cpp

        carinfo/CarInfo.cxx
        #carinfo/Gps.cxx
        #carinfo/GpsPubSubTypes.cxx
        carinfo/CarInfoPublisher.cpp
        carinfo/CarInfoPubSubTypes.cxx
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

add_library(fastcdr SHARED IMPORTED)
add_library(fastrtps SHARED IMPORTED)
add_library(foonathan_memory SHARED IMPORTED)

set_target_properties(fastcdr PROPERTIES IMPORTED_LOCATION ${DDS_PATH}lib/libfastcdr.so)
set_target_properties(fastrtps PROPERTIES IMPORTED_LOCATION ${DDS_PATH}lib/libfastrtps.so)
set_target_properties(foonathan_memory PROPERTIES IMPORTED_LOCATION ${DDS_PATH}lib/libfoonathan_memory-0.7.1.so)

include_directories(${DDS_PATH}include/)
include_directories(src/main/cpp)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        ipa2xwarning
        fastcdr
        fastrtps
        foonathan_memory

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})