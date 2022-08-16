#include "include/common.hpp"

void addCommonDdsArguments(argparse::ArgumentParser& program)
{
    program.add_argument("-s", "--server")
        .help("Use discovery server at given IP address instead of simple (local network multicast) discovery")
        .default_value(std::string("192.168.162.10:11811"));
}
