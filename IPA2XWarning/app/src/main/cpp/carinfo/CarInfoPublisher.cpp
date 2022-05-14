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
    bool init() {

        DomainParticipantQos participantQos;
        participantQos.name("ANDROID PUBLISHER");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);
        if (participant_ == nullptr) { return false; }
        type_.register_type(participant_);
        topic_ = participant_->create_topic("CarInfoTopic", "CarInfoType", TOPIC_QOS_DEFAULT);
        if (topic_ == nullptr) { return false; }
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
        if (publisher_ == nullptr) { return false; }
        writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT);
        if (writer_ == nullptr) { return false; }

        return true;
    }

    //!Send a publication
    bool publish(int speed, double latitude, double longitude) {
        message_.speed(speed);
        message_.coords().latitude(latitude);
        message_.coords().longtitude(longitude);
        writer_->write(&message_);
        return true;
    }
};



extern "C"
JNIEXPORT jlong JNICALL
Java_cz_cvut_fel_marunluk_ipa2xwarning_InfoHandler_initInfoPublisher(JNIEnv *env, jobject thiz) {
    CarInfoPublisher* publisher = new CarInfoPublisher();
    if (publisher->init()) {
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