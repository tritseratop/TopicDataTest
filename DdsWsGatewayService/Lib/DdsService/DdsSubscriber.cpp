#include "Lib/DdsService/DdsSubscriber.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastrtps/Domain.h>
#include <fastrtps/transport/TCPv4TransportDescriptor.h>
#include <fastrtps/transport/UDPv4TransportDescriptor.h>
#include <fastrtps/utils/IPLocator.h>

#include <mutex>

using namespace eprosima::fastdds::dds;
using eprosima::fastrtps::types::ReturnCode_t;

SubscriberService::SubscriberService(const ServiceConfigForTest<SubscriberConfig>& config,
									 std::shared_ptr<DataCacher> cacher)
	: config_(config)
	, participant_(nullptr)
	, cacher_(cacher)
	, config_subscriber_(nullptr)
{
	setVectorSizesInDataTopic();
}

SubscriberService::~SubscriberService()
{
	deleteSubscribers();

	// TODO: надо ли проверять на nullptr??
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

bool SubscriberService::initSubscribers()
{
	setVectorSizesInDataTopic();
	initParticipant();

	if (!config_.configs.empty())
	{
		for (const auto& config : config_.configs)
		{
			initSubscriber(config);
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

bool SubscriberService::initParticipant()
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

DomainParticipantQos SubscriberService::getParticipantQos()
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

		qos.wire_protocol().builtin.discovery_config.leaseDuration = c_TimeInfinite;
		qos.wire_protocol().builtin.discovery_config.leaseDuration_announcementperiod = Duration_t(
			5, 0);
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

bool SubscriberService::initSubscriber(const SubscriberConfig& config)
{
	// TODO: узнать че менять в SUBSCRIBER_QOS_DEFAULT
	AbstractDdsSubscriber* sub = factory_.createSubscriber(
		participant_, config, config.isCache ? cacher_ : nullptr);
	if (sub == nullptr)
	{
		return false;
	}

	std::lock_guard<std::mutex> guard(std::mutex());
	subscribers_.push_back(sub);
	return true;
}

void SubscriberService::deleteSubscribers()
{
	for (auto& sub : subscribers_)
	{
		delete sub;
	}
	subscribers_.clear();
}

void SubscriberService::runSubscribers()
{
	stop_ws_server_ = false;
	std::vector<std::thread> threads;
	analyser_ = PackageAnalyser::getInstance();
	for (auto& sub : subscribers_)
	{
		threads.push_back(std::thread([&]() { sub->run(); }));
	}

	for (auto& t : threads)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
	stop_ws_server_ = true;
	analyser_->writeResults();
	analyser_->clear();
}

void SubscriberService::changeSubscribersConfig(
	const ServiceConfigForTest<SubscriberConfig>& config)
{
	if (config_ == config)
	{
		std::cout << "This subscriber's configuration has been already runConfigPub" << std::endl;
	}
	else
	{
		// TODO сравнение параметров
		config_ = config;
		deleteSubscribers();
		initSubscribers();
	}
}

std::vector<AbstractDdsSubscriber*> SubscriberService::getSubscribers() const
{
	return subscribers_;
}

std::deque<MediateDataDto> SubscriberService::getDataCacheCopy() const
{
	return cacher_->getDataCacheCopy();
}

// TODO: сделать макрос?
void SubscriberService::setVectorSizesInDataTopic()
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