add_library(common Common.cpp)
target_link_libraries(common argparse)

add_executable(InfoPub CarInfoPublisher.cpp)
target_link_libraries(InfoPub common idl fastrtps fastcdr argparse)

add_executable(InfoSub CarInfoSubscriber.cpp)
target_link_libraries(InfoSub common idl fastrtps fastcdr argparse)

add_executable(CrossPub CrossingPublisher.cpp)
target_link_libraries(CrossPub common idl fastrtps fastcdr argparse)

add_executable(CrossSub CrossingSubscriber.cpp)
target_link_libraries(CrossSub common idl fastrtps fastcdr argparse)

add_executable(DiscoveryServer DiscoveryServer.cpp)
target_link_libraries(DiscoveryServer common idl fastrtps fastcdr argparse)
