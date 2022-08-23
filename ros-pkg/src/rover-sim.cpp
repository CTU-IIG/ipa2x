#include "ros/ros.h"
#include "std_msgs/UInt8.h"

#include <sstream>

int main(int argc, char **argv) {
    ros::init(argc, argv, "dummy");
    ros::NodeHandle nh;
    ros::Publisher publisher = nh.advertise<std_msgs::UInt8>("/hipert/state", 1000);
    ros::Rate loop_rate(1);
    int i = 0;
    while (ros::ok()) {
        std_msgs::UInt8 msg;
        uint8_t tosend = (i++ % 5);
        msg.data = tosend;  
        std::cout << "Sent " << tosend + 0 << std::endl;
        publisher.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}