#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <vector>
#include <argparse/argparse.hpp>

constexpr char IPA2X_GUID_PREFIX[] = "69.70.61.32.78.5F.63.76.75.74.be.ef";

void addCommonDdsArguments(argparse::ArgumentParser& program);
std::vector<int> parseIP(const std::string &address, uint16_t default_port = 11811);


#endif
