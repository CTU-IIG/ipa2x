#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/UInt8.h"


#include "common.hpp"
#include "CrossingInfoPublisher.hpp"

#include <sstream>

using namespace eprosima::fastdds::dds;
using namespace std;

CrossingInfoPublisher* publisher;

void callbackP(const std_msgs::UInt8::ConstPtr& msg) {
    if (publisher == nullptr) {
        return;
    }
    if ((msg->data >= 2) && (msg->data <= 4)) {
        publisher->publish(false, true);
    } else {
        publisher->publish(false, false);
    }
    ROS_DEBUG_NAMED("ipa2x_car_connnect", "Received %d", msg->data + 0);
}

int main(int argc, char *argv[]) {

    publisher = nullptr;

    argparse::ArgumentParser program("IPA2X Connect translator");

    addCommonDdsArguments(program);

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    publisher = new CrossingInfoPublisher
        (program.is_used("--server"), parseIP(program.get("--server")));


    ros::init(argc, argv, "ipa2x_car_connect");
    ros::NodeHandle nh;
    ros::Subscriber subscriber = nh.subscribe("/hipert/state", 1000, callbackP);
    ros::spin();
    if (publisher)
        delete publisher;
    return 0;
}