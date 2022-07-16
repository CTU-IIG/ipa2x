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
 * @file CrossingInfoSubscriber.cpp
 *
 */

#include "CrossingInfoPubSubTypes.h"
#include "GpsPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

using namespace eprosima::fastdds::dds;

class CrossingInfoSubscriber {

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
                    if (message_.danger()) {
                        std::cout << "DANGER!";
                    } else if (message_.crossing()) {
                        std::cout << "Crossing occupied";
                    } else {
                        std::cout << "Clear of conflict";
                    }
                    std::cout << " at " << message_.coords().longtitude() << " " << message_.coords().latitude() << std::endl;
                } else {
                    std::cout << "Got invalid data" << std::endl;
                }
            }
        }

        CrossingInfoType message_;
        std::atomic_int samples_;

    } listener_;

public:

    CrossingInfoSubscriber()
        : participant_(nullptr)
        , subscriber_(nullptr)
        , topic_(nullptr)
        , reader_(nullptr)
        , type_(new CrossingInfoTypePubSubType())
    {}

    virtual ~CrossingInfoSubscriber() {
        if (reader_ != nullptr) {
            subscriber_->delete_datareader(reader_);
        } if (topic_ != nullptr) {
            participant_->delete_topic(topic_);
        } if (subscriber_ != nullptr) {
            participant_->delete_subscriber(subscriber_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

    //!Initialize the subscriber
    bool init() {

        // Get default participant QoS
        DomainParticipantQos client_qos = PARTICIPANT_QOS_DEFAULT;

        // Set participant as CLIENT
        client_qos.wire_protocol().builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::CLIENT;

        // Set SERVER's GUID prefix
        eprosima::fastrtps::rtps::RemoteServerAttributes remote_server_att;
        remote_server_att.ReadguidPrefix("44.53.00.5f.45.50.52.4f.53.49.4d.41");

        // Set SERVER's listening locator for PDP
        eprosima::fastrtps::rtps::Locator_t locator;
        // Set SERVER's IP address
        eprosima::fastrtps::rtps::IPLocator::setIPv4(locator, 192, 168, 162, 10);
        locator.port = 11811;
        remote_server_att.metatrafficUnicastLocatorList.push_back(locator);

        // Add remote SERVER to CLIENT's list of SERVERs
        client_qos.wire_protocol().builtin.discovery_config.m_DiscoveryServers.push_back(remote_server_att);

        // Set ping period to 250 ms
        client_qos.wire_protocol().builtin.discovery_config.discoveryServer_client_syncperiod = eprosima::fastrtps::Duration_t(0, 250000000);

        DomainParticipantQos participantQos;
        client_qos.name("Rover dummy SUBSCRIBER");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, client_qos);
        type_.register_type(participant_);
        topic_ = participant_->create_topic("CrossingTopic", "CrossingInfoType", TOPIC_QOS_DEFAULT);
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
        reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);

        return true;
    }

     void run(uint32_t samples) {
        while(listener_.samples_ < samples) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

};


int main() {
    std::cout.precision(10);
    CrossingInfoSubscriber* subscriber = new CrossingInfoSubscriber();
    subscriber->init();
    subscriber->run(10);
    delete subscriber;
    return 0;
}
