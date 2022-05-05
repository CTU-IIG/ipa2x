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

#include <fastdds/rtps/transport/UDPv4TransportDescriptor.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/qos/DomainParticipantQos.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <android/log.h>
#include <jni.h>

jmethodID gMethod;
jobject gObject;
JavaVM *java_vm;

using namespace eprosima::fastdds::dds;

class CrossingInfoSubscriber {

private:
    DomainParticipant* participant_;
    Subscriber* subscriber_;
    DataReader* reader_;
    Topic* topic_;
    TypeSupport type_;


public: class SubListener : public DataReaderListener {

    private: JNIEnv *env;

    public:

        SubListener() : samples_(0) {}

        ~SubListener() override {}

        void on_subscription_matched(DataReader*, const SubscriptionMatchedStatus& info) override {
            __android_log_print(ANDROID_LOG_VERBOSE, "cpptest", "Zmena v poctu publisheru");
        }

        void on_data_available(DataReader* reader) override {

            java_vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
            java_vm->AttachCurrentThread(&env, NULL);

            SampleInfo info;
            if (reader->take_next_sample(&message_, &info) == ReturnCode_t::RETCODE_OK) {
                if (info.valid_data) {
                    bool danger = message_.danger();
                    env->CallVoidMethod(gObject, gMethod, danger);
                    if (message_.danger()) {
                        __android_log_print(ANDROID_LOG_VERBOSE, "cpptest", "NEBEZPEČÍ!");
                    } else {
                        __android_log_print(ANDROID_LOG_VERBOSE, "cpptest", "Nebezpečí nehrozí");
                    }
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
    bool init(std::string _topic = "top") {

        DomainParticipantQos participantQos;
        participantQos.name("ANDROID SUBSCRIBER");
        std::shared_ptr<eprosima::fastdds::rtps::UDPv4TransportDescriptor> descriptor = std::make_shared<eprosima::fastdds::rtps::UDPv4TransportDescriptor>();
        participantQos.transport().user_transports.push_back(descriptor);
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);
        type_.register_type(participant_);
        topic_ = participant_->create_topic(_topic, "CrossingInfoType", TOPIC_QOS_DEFAULT);
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
        //DataReaderQos* qos = new DataReaderQos();
        reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);

        return true;
    }

};


extern "C"
JNIEXPORT jlong JNICALL
Java_cz_cvut_fel_marunluk_ipa2xwarning_MainActivity_initCrossingSubscrier(JNIEnv *env, jobject thiz) {
    CrossingInfoSubscriber* subscriber = new CrossingInfoSubscriber();
    if (subscriber->init("topic")) {
        env->GetJavaVM(&java_vm);
        gObject = env->NewGlobalRef(thiz);
        jclass lClass = env->FindClass("cz/cvut/fel/marunluk/ipa2xwarning/MainActivity");
        gMethod = env->GetMethodID(lClass, "drawDanger", "(Z)V");
        __android_log_print(ANDROID_LOG_VERBOSE, "cpptest", "Init done!");
        return (jlong) subscriber;
    }
    return 0;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_cz_cvut_fel_marunluk_ipa2xwarning_MainActivity_killCrossingSubscriber(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jlong pointer) {
    CrossingInfoSubscriber* subscriber = (CrossingInfoSubscriber *) pointer;
    delete subscriber;
    return true;
}
