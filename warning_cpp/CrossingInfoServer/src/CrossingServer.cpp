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

#include "CrossingInfoPubSubTypes.h"
#include "GpsPubSubTypes.h"

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
    CrossingInfoServer() : participant_(nullptr) {}

    //Destructor
    virtual ~CrossingInfoServer() {
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    //!Initialize the publisher
    bool init() {

        // Get default participant QoS
        DomainParticipantQos server_qos = PARTICIPANT_QOS_DEFAULT;

        // Set participant as SERVER
        server_qos.wire_protocol().builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::SERVER;

        // Set SERVER's GUID prefix
        std::istringstream("44.53.00.5f.45.50.52.4f.53.49.4d.41") >> server_qos.wire_protocol().prefix;

        // Set SERVER's listening locator for PDP
        eprosima::fastrtps::rtps::Locator_t locator;
        // Set SERVER's IP address
        eprosima::fastrtps::rtps::IPLocator::setIPv4(locator, 192, 168, 162, 10);
        locator.port = 11811;
        server_qos.wire_protocol().builtin.metatrafficUnicastLocatorList.push_back(locator);

        /* Add a remote serve to which this server will connect */
        // Set remote SERVER's GUID prefix
        //RemoteServerAttributes remote_server_att;
        //remote_server_att.ReadguidPrefix("44.53.01.5f.45.50.52.4f.53.49.4d.41");

        // Set remote SERVER's listening locator for PDP
        //Locator_t remote_locator;
        //IPLocator::setIPv4(remote_locator, 127, 0, 0, 1);
        //remote_locator.port = 11812;
        //remote_server_att.metatrafficUnicastLocatorList.push_back(remote_locator);

        // Add remote SERVER to SERVER's list of SERVERs
        //server_qos.wire_protocol().builtin.discovery_config.m_DiscoveryServers.push_back(remote_server_att);

        // Create SERVER
        server_qos.name("Rover dummy SUBSCRIBER");
        DomainParticipant* server = DomainParticipantFactory::get_instance()->create_participant(0, server_qos);
        if (nullptr == server) { return false; }

        //server_qos.wire_protocol().participant_id = 1;
        
       //participant_ = DomainParticipantFactory::get_instance()->create_participant(0, server_qos);

        return true;
    }

    void run() {
        std::cout << "Server running. Please press enter to stop the server" << std::endl;
        std::cin.ignore();
    }

};

int main() {

    CrossingInfoServer* server = new CrossingInfoServer();   
    server->init();
    server->run();
    delete server;

}
