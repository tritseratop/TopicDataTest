#include "Lib/DdsService/Service.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastrtps/Domain.h>

#include <fastrtps/transport/TCPv4TransportDescriptor.h>
#include <fastrtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/utils/IPLocator.h>

#include <mutex>

namespace scada_ate::dds::subscriber
{
using namespace eprosima::fastdds;
using namespace eprosima::fastdds::dds;
using eprosima::fastrtps::types::ReturnCode_t;

Service::Service(const ParticipantConfig<Config>& config)
	: config_(config)
	, participant_(nullptr)
	, config_subscriber_(nullptr)
	, stop_(false)
{
	setVectorSizesInDataTopic();
}

Service::~Service()
{
	deleteSubscribers();

	// TODO: ???? ?? ????????? ?? nullptr??
	if (participant_ != nullptr)
	{
		if (participant_->delete_contained_entities() != ReturnCode_t::RETCODE_OK)
		{
			std::cout << "Participant's entities deletion failed" << std::endl;
		}
		if (DomainParticipantFactory::get_instance()->delete_participant(participant_)
			!= ReturnCode_t::RETCODE_OK)
		{
			std::cout << "Participant deletion failed" << std::endl;
		}
	}
}

bool Service::initSubscribers(std::unordered_map<CacheId, std::shared_ptr<void>> cachers)
{
	setVectorSizesInDataTopic();
	initParticipant();

	if (!config_.configs.empty())
	{
		for (const auto& config : config_.configs)
		{
			initSubscriber(config, cachers.at(config.cache_id));
		}
	}
	else
	{
		std::cout << "Configuration for subscribers is not found" << std::endl;
		return false;
	}

	for (auto& sub : subscribers_)
	{
		sub->init();
	}
	return true;
}

bool Service::initParticipant()
{
	if (participant_ == nullptr)
	{
		participant_ = DomainParticipantFactory::get_instance()->create_participant(
			0, getParticipantQos());
		if (participant_ == nullptr)
		{
			return false;
		}
	}
	return true;
}

DomainParticipantQos Service::getParticipantQos()
{
	using namespace eprosima::fastrtps;
	using namespace eprosima::fastrtps::rtps;

	DomainParticipantQos qos;

	if (config_.transport == Transport::TCP)
	{
		Locator_t initial_peer_locator;
		initial_peer_locator.kind = LOCATOR_KIND_TCPv4;
		initial_peer_locator.port = config_.port;

		std::shared_ptr<TCPv4TransportDescriptor> descriptor =
			std::make_shared<TCPv4TransportDescriptor>();

		/*for (std::string ip : config_.whitelist)
		{
			descriptor->interfaceWhiteList.push_back(ip);
			std::cout << "Whitelisted " << ip << std::endl;
		}*/
		IPLocator::setIPv4(initial_peer_locator, config_.ip);
		qos.wire_protocol().builtin.initialPeersList.push_back(
			initial_peer_locator); // Publisher's meta channel

		qos.wire_protocol().builtin.discovery_config.leaseDuration = Duration_t(10, 0);
		qos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = Duration_t(
			2, 0);
		qos.name(config_.participant_name);

		qos.transport().use_builtin_transports = false;

		qos.transport().user_transports.push_back(descriptor);
	}
	else
	{
		Locator_t initial_peer_locator;
		initial_peer_locator.kind = LOCATOR_KIND_UDPv4;
		IPLocator::setIPv4(initial_peer_locator, config_.ip);
		qos.wire_protocol().builtin.initialPeersList.push_back(initial_peer_locator);
		//initial_peer_locator.port = config_.port;

		std::shared_ptr<UDPv4TransportDescriptor> descriptor =
			std::make_shared<UDPv4TransportDescriptor>();

		descriptor->sendBufferSize = 0;
		descriptor->receiveBufferSize = 0;
		descriptor->non_blocking_send = true;

		qos.transport().user_transports.push_back(descriptor);
	}

	return qos;
}

bool Service::initSubscriber(const Config& config, std::shared_ptr<void> cacher)
{
	// TODO: ?????? ?? ?????? ? SUBSCRIBER_QOS_DEFAULT
	AbstractSubscriber* sub = factory_.createSubscriber(participant_, config, cacher);
	if (sub == nullptr)
	{
		return false;
	}

	std::lock_guard<std::mutex> guard(std::mutex());
	subscribers_.push_back(sub);
	return true;
}

void Service::deleteSubscribers()
{
	for (auto& sub : subscribers_)
	{
		delete sub;
	}
	subscribers_.clear();
}

void Service::runSubscribers()
{
	std::vector<std::thread> threads;
	//analyser_ = PackageAnalyser::getInstance();
	while (!stop_)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	/*for (auto& sub : subscribers_)
	{
		threads.push_back(std::thread([&]() { sub->run(); }));
	}

	for (auto& t : threads)
	{
		if (t.joinable())
		{
			t.join();
		}
	}*/
	//analyser_->writeResultsAndClear("");
}

void Service::stopSubscribers()
{
	stop_ = true;
	/*for (auto& sub : subscribers_)
	{
		sub->stop();
	}*/
}

void Service::changeSubscribersConfig(const ParticipantConfig<Config>& config)
{
	if (config_ == config)
	{
		std::cout << "This subscriber's configuration has been already runConfigPub" << std::endl;
	}
	else
	{
		// TODO ????????? ??????????
		config_ = config;
		deleteSubscribers();
		//initSubscribers();
	}
}

std::vector<AbstractSubscriber*> Service::getSubscribers() const
{
	return subscribers_;
}

// TODO: ??????? ???????
void Service::setVectorSizesInDataTopic()
{
	scada_ate::typetopics::SetMaxSizeDataChar(config_.MaxSizeSizeDataChar);
	scada_ate::typetopics::SetMaxSizeDataCollectionInt(config_.MaxSizeDataCollectionInt);
	scada_ate::typetopics::SetMaxSizeDataCollectionFloat(config_.MaxSizeDataCollectionFloat);
	scada_ate::typetopics::SetMaxSizeDataCollectionDouble(config_.MaxSizeDataCollectionDouble);
	scada_ate::typetopics::SetMaxSizeDataCollectionChar(config_.MaxSizeDataCollectionChar);

	scada_ate::typetopics::SetMaxSizeDataExVectorChar(config_.MaxSizeSizeDataExVectorChar);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorInt(config_.MaxSizeDDSDataExVectorInt);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorFloat(config_.MaxSizeDDSDataExVectorFloat);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorDouble(config_.MaxSizeDDSDataExVectorDouble);
	scada_ate::typetopics::SetMaxSizeDDSDataExVectorChar(config_.MaxSizeDDSDataExVectorChar);

	scada_ate::typetopics::SetMaxSizeDDSAlarmExVectorAlarms(config_.MaxSizeDDSAlarmVectorAlarm);

	scada_ate::typetopics::SetMaxSizeDDSAlarmExVectorAlarms(config_.MaxSizeDDSExVectorAlarms);
}
} // namespace scada_ate::dds::subscriber