#include "include/common.hpp"

void addCommonDdsArguments(argparse::ArgumentParser& program)
{
    program.add_argument("-i", "--ip")
        .help("IP address of the discovery server [xxx.xxx.xxx.xxx[:port]]")
        .default_value(std::string("127.0.0.1:11811"));

    program.add_argument("-s", "--server")
        .help("Use discovery server instead of simple (local network multicast) discovery")
        .default_value(false)
        .implicit_value(true);
}
