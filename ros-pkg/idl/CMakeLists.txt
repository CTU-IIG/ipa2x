
add_library(idl STATIC
  CarInfo.cxx CarInfoPubSubTypes.cxx CrossingInfo.cxx
  CrossingInfoPubSubTypes.cxx Gps.cxx GpsPubSubTypes.cxx)

target_link_libraries(idl fastrtps fastcdr)
target_include_directories(idl PUBLIC .)
