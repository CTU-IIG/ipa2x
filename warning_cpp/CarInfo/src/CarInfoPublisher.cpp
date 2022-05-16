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

    CarInfoPublisher()
        : participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new CarInfoTypePubSubType()) {
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

    //!Initialize the publisher
    bool init() {

        DomainParticipantQos participantQos;
        participantQos.name("Rover dummy PUBLISHER");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);
        type_.register_type(participant_);
        topic_ = participant_->create_topic("CarInfoTopic", "CarInfoType", TOPIC_QOS_DEFAULT);
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
        writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT);

        return true;
    }

    //!Send a publication
    void publish(int value) {
        std::cout << "Publishing\n";
        message_.speed(value);
        if (value == 0) {
            value = 10000000;
        }
        message_.coords().latitude((5/value) + 50.1027778F);
        message_.coords().longtitude((5/value) + 14.3945306F);
        writer_->write(&message_);
    }

};

int main() {

    CarInfoPublisher* publisher = new CarInfoPublisher();   
    publisher->init();
    std::cout << "Type speed (will be used as GPS modifier as well)" << std::endl;
    std::string c;
    while (1) {
        std::cin >> c;
        if (c.compare("exit") == 0) {
            break;
        } else {
            publisher->publish(stoi(c));
        }
    }
    

    delete publisher;

}
