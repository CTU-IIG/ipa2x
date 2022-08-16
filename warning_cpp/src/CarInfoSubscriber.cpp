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
 * @file CarInfoSubscriber.cpp
 *
 */

#include "CarInfoPubSubTypes.h"
#include "GpsPubSubTypes.h"
#include "common.hpp"

#include <argparse/argparse.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

using namespace eprosima::fastdds::dds;

class CarInfoSubscriber {

private:
    DomainParticipant* participant_;
    Subscriber* subscriber_;
    DataReader* reader_;
    Topic* topic_;
    TypeSupport type_;


public: class SubListener : public DataReaderListener {

    public:

        SubListener() : samples_(0) {
            std::cout << "Starting" << std::endl;
        }

        ~SubListener() override {}

        void on_subscription_matched(DataReader*, const SubscriptionMatchedStatus& info) override {
            std::cout << "Number of publishers changed" << std::endl;
        }

        void on_data_available(DataReader* reader) override {

            SampleInfo info;
            if (reader->take_next_sample(&message_, &info) == ReturnCode_t::RETCODE_OK) {
                if (info.valid_data) {
                    samples_++;
                    std::cout << "Car at " << message_.coords().longitude() << " " << message_.coords().latitude() << " going " << message_.speed() << " km/h" << std::endl;
                } else {
                    std::cout << "Got invalid data" << std::endl;
                }
            }
        }

        CarInfoType message_;
        std::atomic_int samples_;

    } listener_;

public:

    CarInfoSubscriber(bool server, std::vector<int> ip)
        : participant_(nullptr)
        , subscriber_(nullptr)
        , topic_(nullptr)
        , reader_(nullptr)
        , type_(new CarInfoTypePubSubType())
    {

        DomainParticipantQos participantQos = PARTICIPANT_QOS_DEFAULT;

        if (server) {

            // Set participant as CLIENT
            participantQos.wire_protocol().builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::CLIENT;

            // Set SERVER's GUID prefix
            eprosima::fastrtps::rtps::RemoteServerAttributes remote_server_att;
            remote_server_att.ReadguidPrefix(IPA2X_GUID_PREFIX);;

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

        participantQos.name("Dummy rover SUBSCRIBER");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(IPA2X_DOMAIN_ID, participantQos);
        type_.register_type(participant_);
        topic_ = participant_->create_topic("CarInfoTopic", "CarInfoType", TOPIC_QOS_DEFAULT);
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
        DataReaderQos drqos = DATAREADER_QOS_DEFAULT;
        drqos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
        drqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        reader_ = subscriber_->create_datareader(topic_, drqos, &listener_);
    }

    virtual ~CarInfoSubscriber() {
        if (reader_ != nullptr) {
            subscriber_->delete_datareader(reader_);
        } if (topic_ != nullptr) {
            participant_->delete_topic(topic_);
        } if (subscriber_ != nullptr) {
            participant_->delete_subscriber(subscriber_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }


     void run(uint32_t samples) {
        while(listener_.samples_ < samples || samples == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

};


int main(int argc, char *argv[]) {

    argparse::ArgumentParser program("InfoSub");

    program.add_argument("count")
        .help("Set number of expexted messages. 0 or no value will result in infinite amount of samples")
        .scan<'i', int>()
        .default_value(0);

    addCommonDdsArguments(program);

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    std::cout.precision(10);
    CarInfoSubscriber* subscriber = new CarInfoSubscriber(program.is_used("--server"),
                                                          parseIP(program.get("--server")));
    subscriber->run(program.get<int>("count"));
    delete subscriber;
    return 0;

}
