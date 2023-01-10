add_library(common Common.cpp)
target_link_libraries(common argparse::argparse fastrtps fastcdr)

add_executable(InfoPub CarInfoPublisher.cpp)
target_link_libraries(InfoPub common idl fastrtps fastcdr argparse::argparse)
install(TARGETS InfoPub)

add_executable(InfoSub CarInfoSubscriber.cpp)
target_link_libraries(InfoSub common idl fastrtps fastcdr argparse::argparse)
install(TARGETS InfoSub)

add_executable(CrossPub CrossPub.cpp)
target_link_libraries(CrossPub common idl fastrtps fastcdr argparse::argparse)
install(TARGETS CrossPub)

add_executable(CrossSub CrossingSubscriber.cpp)
target_link_libraries(CrossSub common idl fastrtps fastcdr argparse::argparse)
install(TARGETS CrossSub)

add_executable(DiscoveryServer DiscoveryServer.cpp)
target_link_libraries(DiscoveryServer common idl fastrtps fastcdr argparse::argparse)
install(TARGETS DiscoveryServer)
