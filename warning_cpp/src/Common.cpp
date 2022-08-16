#include "common.hpp"

void addCommonDdsArguments(argparse::ArgumentParser& program)
{
    program.add_argument("-s", "--server")
        .help("Use discovery server at given IP address instead of simple (local network multicast) discovery")
        .default_value(std::string("192.168.162.10:11811"));
}

std::vector<int> parseIP(std::string address) {

    std::size_t portPos = address.find(':');
    std::string _ip;
    std::string _port;
    std::vector<int> addr = {127, 0, 0, 1, 11811};

    if (portPos != std::string::npos) {
        _ip = address.substr(0, portPos);
        _port = address.substr(portPos+1, address.length()-1);

        if ((1 != sscanf(_port.c_str(), "%u", &addr[4])) || (addr[4] < 1024) || (addr[4] > 65535)) {
            addr[4] = 11811;
        }

    } else {
        _ip = address;
    }

    if ((4 != sscanf(_ip.c_str(), "%u.%u.%u.%u", &addr[0], &addr[1], &addr[2], &addr[3])) || (addr[0] > 255) || (addr[1] > 255) || (addr[2] > 255) || (addr[3] > 255)) {
        addr[0] = 127;
        addr[1] = 0;
        addr[2] = 0;
        addr[3] = 1;
    }

    return addr;
}
