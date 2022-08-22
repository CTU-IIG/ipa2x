#include "common.hpp"
#include <exception>

using namespace std;

void addCommonDdsArguments(argparse::ArgumentParser& program)
{
    program.add_argument("-s", "--server")
        .help("Use discovery server at given IP address instead of simple (local network multicast) discovery")
        .default_value(std::string("192.168.162.10:11811"));
}

std::vector<int> parseIP(const std::string &address, uint16_t default_port) {

    std::size_t portPos = address.find(':');
    std::string _ip;
    std::string _port;
    std::vector<int> addr = {0, 0, 0, 0, default_port};

    if (portPos != std::string::npos) {
        _ip = address.substr(0, portPos);
        _port = address.substr(portPos+1, address.length()-1);
    } else {
        _ip = address;
    }

    if ((4 != sscanf(_ip.c_str(), "%u.%u.%u.%u", &addr[0], &addr[1], &addr[2], &addr[3])) ||
        (addr[0] > 255) || (addr[1] > 255) || (addr[2] > 255) || (addr[3] > 255)) {
            throw runtime_error("Cannot parse IP address " + address);
    }

    if (_port.length() > 0 &&
        (1 != sscanf(_port.c_str(), "%u", &addr[4])) || (addr[4] > 65535)) {
            throw runtime_error("Cannot parse port in " + address);
    }

    return addr;
}
