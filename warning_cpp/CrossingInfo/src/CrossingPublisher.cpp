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

using namespace eprosima::fastdds::dds;

class CrossingInfoPublisher {
private:

    CrossingInfoType message_;
    DomainParticipant* participant_;
    Publisher* publisher_;
    Topic* topic_;
    DataWriter* writer_;
    TypeSupport type_;


public:

    CrossingInfoPublisher()
        : participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new CrossingInfoTypePubSubType()) {
    }

    virtual ~CrossingInfoPublisher() {

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
        topic_ = participant_->create_topic("CrossingTopic", "CrossingInfoType", TOPIC_QOS_DEFAULT);
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
        writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT);

        return true;
    }

    //!Send a publication
    void publish(bool danger, bool crossing) {
        std::cout << "Publishing\n";
        message_.danger(danger);
        message_.crossing(crossing);
        message_.coords().latitude(50.1027778F);
        message_.coords().longtitude(14.3945306F);
        writer_->write(&message_);
    }

};

int main() {

    CrossingInfoPublisher* publisher = new CrossingInfoPublisher();   
    publisher->init();
    std::cout << "x: crossing" << std::endl << "c: clear" << std::endl << "d: danger" << std::endl << "q: quit" << std::endl;
    char c;
    while (std::cin >> c) {
        if (c == 'C' || c == 'c') {
            publisher->publish(false, false);
        } else if (c == 'X' || c == 'x') {
            publisher->publish(false, true);
        } else if (c == 'D' || c == 'd') {
            publisher->publish(true, true);
        } else if (c == 'Q' || c == 'q') {
            break;
        }
    }
    

    delete publisher;

}
