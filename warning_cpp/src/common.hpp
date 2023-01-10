// Copyright 2022, 2023 Czech Technical University in Prague
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <vector>
#include <argparse/argparse.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

constexpr char IPA2X_GUID_PREFIX[] = "69.70.61.32.78.5F.63.76.75.74.be.ef";

constexpr eprosima::fastdds::dds::DomainId_t IPA2X_DOMAIN_ID = 2;

void addCommonDdsArguments(argparse::ArgumentParser& program);
std::vector<int> parseIP(const std::string &address, uint16_t default_port = 11811);


#endif
