// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/**
 * @file CrossingInfoPublisher.cpp
 *
 */

#include <argparse/argparse.hpp>
#include <arpa/inet.h>
#include <unistd.h>
#include "include/parse.hpp"
#include "include/common.hpp"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>
#include <fastdds/rtps/common/Locator.h>
#include <fastdds/dds/domain/DomainParticipant.hpp>

using namespace eprosima::fastdds::dds;



class CrossingInfoServer {
private:
    DomainParticipant* participant_;
public:

    //Constructor
    CrossingInfoServer(std::vector<int> ip) : participant_(nullptr) {

        // Get default participant QoS
        DomainParticipantQos server_qos = PARTICIPANT_QOS_DEFAULT;
        // Set participant as SERVER
        server_qos.wire_protocol().builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::SERVER;
        // Set SERVER's GUID prefix
        std::istringstream(IPA2X_GUID_PREFIX) >> server_qos.wire_protocol().prefix;
        // Set SERVER's listening locator for PDP
        eprosima::fastrtps::rtps::Locator_t locator;
        // Set SERVER's IP address
        eprosima::fastrtps::rtps::IPLocator::setIPv4(locator, ip[0], ip[1], ip[2], ip[3]);
        locator.port = ip[4];
        server_qos.wire_protocol().builtin.metatrafficUnicastLocatorList.push_back(locator);

        // Create SERVER
        server_qos.name("Rover dummy SUBSCRIBER");
        DomainParticipant* server = DomainParticipantFactory::get_instance()->create_participant(0, server_qos);
        printf("Starting server on %u.%u.%u.%u:%u\n", ip[0], ip[1], ip[2], ip[3], ip[4]);

    }

    //Destructor
    virtual ~CrossingInfoServer() {
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }



    void run() {
        std::cout << "Server running. Please press enter to stop the server" << std::endl;
        pause();
    }

};


int main(int argc, char *argv[]) {

    argparse::ArgumentParser program("DiscoveryServer");
    std::vector<int> ip = {127, 0, 0, 1, 11811};

    program.add_argument("-i", "--ip")
        .help("Enter IP address of server machine [xxx.xxx.xxx.xxx] defaults to 127.0.0.1");

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    if (auto input = program.present("-i")) {        
        ip = parseIP(input.value());
    
    }

    CrossingInfoServer* server = new CrossingInfoServer(ip);   
    server->run();
    delete server;

}
