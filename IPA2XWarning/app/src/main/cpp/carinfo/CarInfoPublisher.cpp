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
#include <jni.h>

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
    bool init(bool server, std::vector<int> ip) {

        DomainParticipantQos participantQos = PARTICIPANT_QOS_DEFAULT;

        if (server) {

            // Set participant as CLIENT
            participantQos.wire_protocol().builtin.discovery_config.discoveryProtocol = eprosima::fastrtps::rtps::DiscoveryProtocol_t::CLIENT;

            // Set SERVER's GUID prefix
            eprosima::fastrtps::rtps::RemoteServerAttributes remote_server_att;
            remote_server_att.ReadguidPrefix("69.70.61.32.78.5F.63.76.75.74.be.ef");

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
        }

        participantQos.name("ANDROID PUBLISHER");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(2, participantQos);
        if (participant_ == nullptr) { return false; }
        type_.register_type(participant_);
        topic_ = participant_->create_topic("CarInfoTopic", "CarInfoType", TOPIC_QOS_DEFAULT);
        if (topic_ == nullptr) { return false; }
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
        if (publisher_ == nullptr) { return false; }
        DataWriterQos dwqos = DATAWRITER_QOS_DEFAULT;
        dwqos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
        dwqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        writer_ = publisher_->create_datawriter(topic_, dwqos);
        if (writer_ == nullptr) { return false; }

        return true;
    }

    //!Send a publication
    bool publish(int speed, double latitude, double longitude) {
        message_.speed(speed);
        message_.coords().latitude(latitude);
        message_.coords().longitude(longitude);
        writer_->write(&message_);
        return true;
    }
};



extern "C"
JNIEXPORT jlong JNICALL
Java_cz_cvut_fel_marunluk_ipa2xwarning_InfoHandler_initInfoPublisher(JNIEnv *env, jobject thiz,
                                                                     jboolean server, jint ip_a,
                                                                     jint ip_b, jint ip_c,
                                                                     jint ip_d, jint port) {
    CarInfoPublisher* publisher = new CarInfoPublisher();
    std::vector<int> addr = {ip_a, ip_b, ip_c, ip_d, port};
    if (publisher->init(false, addr)) {
        return (jlong) publisher;
    }
    delete publisher;
    return 0;
}
extern "C"
JNIEXPORT jlong JNICALL
Java_cz_cvut_fel_marunluk_ipa2xwarning_InfoHandler_killInfoPublisher(JNIEnv *env, jobject thiz,
                                                                     jlong pointer) {
    CarInfoPublisher* publisher = (CarInfoPublisher*) pointer;
    if (publisher != nullptr) {
        delete publisher;
        return true;
    }
    return false;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_cz_cvut_fel_marunluk_ipa2xwarning_InfoHandler_sendInfoPublisher(JNIEnv *env, jobject thiz,
                                                                     jlong pointer,
                                                                     jdouble longitude,
                                                                     jdouble latitude, jint speed) {
    CarInfoPublisher* publisher = (CarInfoPublisher*) pointer;
    if (publisher != nullptr) {
        return publisher->publish(speed, latitude, longitude);
    }
    return false;
}
