#include "ros/ros.h"
#include "std_msgs/UInt8.h"


#include "CrossingInfoPubSubTypes.h"
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
#include <fastdds/rtps/common/Locator.h>
#include <fastdds/dds/domain/DomainParticipant.hpp>



#include <sstream>

using namespace eprosima::fastdds::dds;
using namespace std;

class CrossingInfoPublisher {
private:

    CrossingInfoType message_;
    DomainParticipant* participant_;
    Publisher* publisher_;
    Topic* topic_;
    DataWriter* writer_;
    TypeSupport type_;


public:

    CrossingInfoPublisher(bool server, std::vector<int> ip)
        : participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new CrossingInfoTypePubSubType()) {

        DomainParticipantQos participantQos = PARTICIPANT_QOS_DEFAULT;

        if (server) {
            printf("Using discovery server at %u.%u.%u.%u:%u\n", ip[0], ip[1], ip[2], ip[3], ip[4]);

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
        } else {
            std::cout << "Using simple discovery" << std::endl;
        }

        participantQos.name("Rover dummy PUBLISHER");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(IPA2X_DOMAIN_ID, participantQos);
        type_.register_type(participant_);
        topic_ = participant_->create_topic("CrossingTopic", "CrossingInfoType", TOPIC_QOS_DEFAULT);
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
        DataWriterQos dwqos = DATAWRITER_QOS_DEFAULT;
        dwqos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
        dwqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
        writer_ = publisher_->create_datawriter(topic_, dwqos);
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

    //!Send a publication
    void publish(bool danger, bool crossing) {
        std::cout << "Publishing\n";
        message_.danger(danger);
        message_.crossing(crossing);
        message_.coords().latitude(50.1027778F);
        message_.coords().longitude(14.3945306F);
        writer_->write(&message_);
    }
} *publisher;

void callbackP(const std_msgs::UInt8::ConstPtr& msg) {
    if (publisher == nullptr) {
        return;
    }
    if (msg->data > 2) {
        publisher->publish(false, true);
    } else {
        publisher->publish(false, false);
    }
    std::cout << "Received " << msg->data + 0 << std::endl;
}

int main(int argc, char *argv[]) {

    publisher = nullptr;

    argparse::ArgumentParser program("IPA2X Connect translator");

    addCommonDdsArguments(program);

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    publisher = new CrossingInfoPublisher(program.is_used("--server"),
                                  parseIP(program.get("--server")));


    ros::init(argc, argv, "translator");
    ros::NodeHandle nh;
    ros::Subscriber subscriber = nh.subscribe("/hipert/state", 1000, callbackP);
    ros::spin();
    if (publisher) {
        delete publisher;
    }
    return 0;
}