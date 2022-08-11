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

jmethodID gWarningMethod;
jmethodID gWarningInfoMethod;
jobject gObject;
JavaVM *java_vm;
static pthread_key_t key;

using namespace eprosima::fastdds::dds;

static void thread_destructor(void* ptr) {
    JNIEnv* env = static_cast<JNIEnv *>(ptr);
    __android_log_print(ANDROID_LOG_VERBOSE, "Sub attached", "About to detach!");
    java_vm->DetachCurrentThread();
    __android_log_print(ANDROID_LOG_VERBOSE, "Sub attached", "Detached!");
}

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

        SubListener() {
            current_endpoints = 0;
        }

        ~SubListener() override {}

        void on_subscription_matched(DataReader*, const SubscriptionMatchedStatus& info) override {

            java_vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
            java_vm->AttachCurrentThread(&env, NULL);
            pthread_setspecific(key, env);

            if (info.current_count_change == 1 || info.current_count_change == -1) {
                current_endpoints += info.current_count_change;
                env->CallVoidMethod(gObject, gWarningInfoMethod, current_endpoints.load());
            }
        }

        void on_data_available(DataReader* reader) override {

            java_vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
            java_vm->AttachCurrentThread(&env, NULL);
            pthread_setspecific(key, env);

            SampleInfo info;
            if (reader->take_next_sample(&message_, &info) == ReturnCode_t::RETCODE_OK) {
                if (info.valid_data) {
                    bool danger = message_.danger();
                    bool crossing = message_.crossing();
                    double lo = message_.coords().longitude();
                    double la = message_.coords().latitude();
                    env->CallVoidMethod(gObject, gWarningMethod, danger, crossing, lo, la);
                }
            }
        }

        CrossingInfoType message_;
        std::atomic_int current_endpoints;

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

        participantQos.name("ANDROID SUBSCRIBER");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(2, participantQos);
        if (participant_ == nullptr) { return false; }
        type_.register_type(participant_);
        topic_ = participant_->create_topic("CrossingTopic", "CrossingInfoType", TOPIC_QOS_DEFAULT);
        if (topic_ == nullptr) { return false; }
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
        if (subscriber_ == nullptr) { return false; }
        DataReaderQos drqos = DATAREADER_QOS_DEFAULT;
        drqos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
        drqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        reader_ = subscriber_->create_datareader(topic_, drqos, &listener_);
        if (reader_ == nullptr) { return false; }

        return true;

    }
};



extern "C"
JNIEXPORT jlong JNICALL
Java_cz_cvut_fel_marunluk_ipa2xwarning_CrossingHandler_initCrossingSubscrier(JNIEnv *env, jobject thiz,
                                                                             jboolean server,
                                                                             jint ip_a, jint ip_b,
                                                                             jint ip_c, jint ip_d,
                                                                             jint port) {
    CrossingInfoSubscriber* subscriber = new CrossingInfoSubscriber();
    std::vector<int> addr = {ip_a, ip_b, ip_c, ip_d, port};
    if (subscriber->init(server, addr)) {
        pthread_key_create(&key, thread_destructor);
        env->GetJavaVM(&java_vm);
        gObject = env->NewGlobalRef(thiz);
        jclass lClass = env->FindClass("cz/cvut/fel/marunluk/ipa2xwarning/CrossingHandler");
        gWarningMethod = env->GetMethodID(lClass, "parseCrossing", "(ZZDD)V");
        gWarningInfoMethod = env->GetMethodID(lClass, "updateCrossingPublisherInfo", "(I)V");
        __android_log_print(ANDROID_LOG_VERBOSE, "CrossingSubscriber", "Init done!");
        return (jlong) subscriber;
    }
    delete subscriber;
    return 0;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_cz_cvut_fel_marunluk_ipa2xwarning_CrossingHandler_killCrossingSubscriber(JNIEnv *env, jobject thiz, jlong pointer) {
    CrossingInfoSubscriber* subscriber = (CrossingInfoSubscriber *) pointer;
    if (subscriber != nullptr) {
        delete subscriber;
        return true;
    }
    return false;
}
