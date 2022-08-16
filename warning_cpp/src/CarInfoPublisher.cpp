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
 * @file CarInfoPublisher.cpp
 *
 */

#include "CarInfoPubSubTypes.h"
#include "GpsPubSubTypes.h"
#include "common.hpp"

#include <argparse/argparse.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>

using namespace eprosima::fastdds::dds;

class CarInfoPublisher {
private:

    CarInfoType message_;
    DomainParticipant* participant_;
    Publisher* publisher_;
    Topic* topic_;
    DataWriter* writer_;
    TypeSupport type_;


public:

    CarInfoPublisher(bool server, std::vector<int> ip)
        : participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new CarInfoTypePubSubType()) {

        DomainParticipantQos participantQos = PARTICIPANT_QOS_DEFAULT;

        if (server) {

            // Set participant as CLIENT
            participantQos.wire_protocol().builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::CLIENT;

            // Set SERVER's GUID prefix
            eprosima::fastrtps::rtps::RemoteServerAttributes remote_server_att;
            remote_server_att.ReadguidPrefix(IPA2X_GUID_PREFIX);

            // Set SERVER's listening locator for PDP
            eprosima::fastrtps::rtps::Locator_t locator;
            // Set SERVER's IP address
            eprosima::fastrtps::rtps::IPLocator::setIPv4(locator, ip[0], ip[1], ip[2], ip[3]);
            locator.port = ip[4];
            remote_server_att.metatrafficUnicastLocatorList.push_back(locator);

            // Add remote SERVER to CLIENT's list of SERVERs
            participantQos.wire_protocol().builtin.discovery_config.m_DiscoveryServers.push_back(remote_server_att);

            // Set ping period to 250 ms
            participantQos.wire_protocol().builtin.discovery_config.discoveryServer_client_syncperiod = eprosima::fastrtps::Duration_t(0, 250000000);

            printf("Starting subscriber with server discovery expected at %u.%u.%u.%u:%u\n", ip[0], ip[1], ip[2], ip[3], ip[4]);

        } else {
            std::cout << "Startrig subscriber with simple discovery" << std::endl;
        }

        participantQos.name("Rover dummy PUBLISHER");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(IPA2X_DOMAIN_ID, participantQos);
        type_.register_type(participant_);
        topic_ = participant_->create_topic("CarInfoTopic", "CarInfoType", TOPIC_QOS_DEFAULT);
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
        DataWriterQos dwqos = DATAWRITER_QOS_DEFAULT;
        dwqos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
        dwqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        writer_ = publisher_->create_datawriter(topic_, dwqos);

    }

    virtual ~CarInfoPublisher() {

        if (writer_ != nullptr) {
            publisher_->delete_datawriter(writer_);
        } if (publisher_ != nullptr) {
            participant_->delete_publisher(publisher_);
        } if (topic_ != nullptr) {
            participant_->delete_topic(topic_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    //!Send a publication
    void publish(int value) {
        std::cout << "Publishing\n";
        message_.speed(value);
        if (value == 0) {
            value = 10000000;
        }
        message_.coords().latitude((5/value) + 50.1027778F);
        message_.coords().longitude((5/value) + 14.3945306F);
        writer_->write(&message_);
    }

};


int main(int argc, char *argv[]) {

    argparse::ArgumentParser program("InfoPub");

    addCommonDdsArguments(program);

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    CarInfoPublisher* publisher = new CarInfoPublisher(program.is_used("--server"),
                                                       parseIP(program.get("--server")));
    std::cout << "Type speed (will be used as GPS modifier as well). Type 'Q' to exit." << std::endl;
    std::string c;
    while (1) {
        std::cin >> c;
        if (c.compare("q") == 0 || c.compare("Q") == 0) {
            break;
        } else {
            publisher->publish(stoi(c));
        }
    }

    delete publisher;
    return 0;

}