#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include "Lib/DdsService/SubscriberFactory.h"

using namespace eprosima::fastdds::dds;

bool operator==(const SubscriberConfig& lhs, const SubscriberConfig& rhs)
{
	return lhs.subscriber_id == rhs.subscriber_id && lhs.vector_size == rhs.subscriber_id
		   && lhs.topic_name == rhs.topic_name && lhs.topic_type_name == rhs.topic_type_name
		   && lhs.topic_type == rhs.topic_type;
}

AbstractDdsSubscriber*
SubscriberFactory::createSubscriber(eprosima::fastdds::dds::DomainParticipant* participant,
									const SubscriberConfig& config,
									std::shared_ptr<void> cacher,
									std::shared_ptr<OnTopicReceived> on_topic_received) const
{
	switch (config.topic_type)
	{
	case TopicType::DDS_DATA:
		return new ConcreteSubscriber<DDSData, DDSDataPubSubType, DataCacher>(
			participant, config, std::static_pointer_cast<DataCacher>(cacher), on_topic_received);
	case TopicType::DDS_DATA_EX:
		return new ConcreteSubscriber<DDSDataEx, DDSDataExPubSubType, DataCacher>(
			participant, config, std::static_pointer_cast<DataCacher>(cacher), on_topic_received);
	case TopicType::DDS_ALARM:
		return new ConcreteSubscriber<DDSAlarm, DDSAlarmPubSubType, AlarmCacher>(
			participant, config, std::static_pointer_cast<AlarmCacher>(cacher), on_topic_received);
	case TopicType::DDS_EX_ALARM:
		return new ConcreteSubscriber<DDSAlarmEx, DDSAlarmExPubSubType, AlarmCacher>(
			participant, config, std::static_pointer_cast<AlarmCacher>(cacher), on_topic_received);
	default:
		std::cout << "Topic type " << config.topic_type_name << " is not found" << std::endl;
		return nullptr;
	}
}

template<class T, class TPubSubType, class Cacher>
ConcreteSubscriber<T, TPubSubType, Cacher>::ConcreteSubscriber(
	eprosima::fastdds::dds::DomainParticipant* participant,
	const SubscriberConfig& config,
	std::shared_ptr<Cacher> cacher,
	std::shared_ptr<OnTopicReceived> on_topic_received)
	: participant_(participant)
	, subscriber_(nullptr)
	, reader_(nullptr)
	, topic_(nullptr)
	, support_type_(new TPubSubType())
	, config_(config)
	, cacher_(cacher)
	, stop_(false)
	, listener_(this)
	, on_topic_received_(on_topic_received)
{ }

template<class T, class TPubSubType, class Cacher>
ConcreteSubscriber<T, TPubSubType, Cacher>::~ConcreteSubscriber()
{
	if (subscriber_ != nullptr)
	{
		if (reader_ != nullptr)
		{
			auto res = subscriber_->delete_datareader(reader_);
			if (res != ReturnCode_t::RETCODE_OK)
			{
				std::cout << "Error: " + std::to_string(res());
			}
		}
		subscriber_->delete_contained_entities();
		auto res = participant_->delete_subscriber(subscriber_);
		if (res != ReturnCode_t::RETCODE_OK)
		{
			std::cout << "Error: " + std::to_string(res());
		}
	}
	if (topic_ != nullptr)
	{
		auto res = participant_->delete_topic(topic_);
		if (res != ReturnCode_t::RETCODE_OK)
		{
			std::cout << "Error: " + std::to_string(res());
		}
	}
}

template<class T, class TPubSubType, class Cacher>
bool ConcreteSubscriber<T, TPubSubType, Cacher>::init()
{
	stop_ = false;

	if (participant_ == nullptr)
	{
		return false;
	}

	support_type_.register_type(participant_);

	topic_ = participant_->create_topic(
		config_.topic_name, config_.topic_type_name, eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);
	if (topic_ == nullptr)
	{
		return false;
	}

	subscriber_ = participant_->create_subscriber(eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT,
												  nullptr);
	if (subscriber_ == nullptr)
	{
		return false;
	}

	eprosima::fastdds::dds::DataReaderQos rqos;

	rqos.history().kind = eprosima::fastdds::dds::KEEP_LAST_HISTORY_QOS;
	rqos.history().depth = 30;
	rqos.resource_limits().max_samples = 50;
	rqos.resource_limits().allocated_samples = 20;
	rqos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
	rqos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;
	rqos.deadline().period.nanosec = config_.sleep * 1000;

	reader_ = subscriber_->create_datareader(topic_, rqos, &listener_);
	if (reader_ == nullptr)
	{
		return false;
	}

	return true;
}

template<class T, class TPubSubType, class Cacher>
void ConcreteSubscriber<T, TPubSubType, Cacher>::run()
{
	while (!stop_)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(config_.sleep));
	}
}

template<class T, class TPubSubType, class Cacher>
void ConcreteSubscriber<T, TPubSubType, Cacher>::stop()
{
	stop_ = true;
}

template<class T, class TPubSubType, class Cacher>
void ConcreteSubscriber<T, TPubSubType, Cacher>::setConfig(const SubscriberConfig& config)
{
	config_ = config;
}

template<class T, class TPubSubType, class Cacher>
void ConcreteSubscriber<T, TPubSubType, Cacher>::cacheData(T data_)
{
	if (cacher_ != nullptr)
	{
		cacher_->cache(std::move(data_));
	}
}

template<class T, class TPubSubType, class Cacher>
void ConcreteSubscriber<T, TPubSubType, Cacher>::setTestCallback(OnTopicReceived on_topic_received)
{
	*on_topic_received_ = on_topic_received;
}

template<class T, class TPubSubType, class Cacher>
ConcreteSubscriber<T, TPubSubType, Cacher>::SubscriberListener::SubscriberListener(
	ConcreteSubscriber* subscriber)
	: sub_(subscriber)
	, samples_(0)
	, matched_(0)
	, first_(false)
{ }

template<class T, class TPubSubType, class Cacher>
ConcreteSubscriber<T, TPubSubType, Cacher>::SubscriberListener::~SubscriberListener()
{ }

//void on_data_available(eprosima::fastdds::dds::DataReader* reader)
//{
//	eprosima::fastdds::dds::SampleInfo info;
//	auto res = reader->take_next_sample(&data_sample_, &info);
//	if (res == ReturnCode_t::RETCODE_OK)
//	{
//		auto timestamp = TimeConverter::GetTime_LLmcs();
//		if (!first_)
//		{
//			analyser_ = PackageAnalyser::getInstance();
//			analyser_->setInitialInfo(sub_->config_.topic_type_name + " "
//									  + std::to_string(sub_->config_.vector_size));
//			first_ = true;
//		}
//		analyser_->pushPackageTimestamp({data_sample_.time_service(),
//										 timestamp,
//										 timestamp - data_sample_.time_service(),
//										 T::getCdrSerializedSize(data_sample_)});
//		if (info.valid_data)
//		{
//			samples_++;
//			//TODO по другому надо как то проверять
//			{
//				if (samples_ >= sub_->config_.samples)
//				{
//					sub_->stop_ = true;
//				}
//				sub_->cacheData(data_sample_);
//			}
//			std::cout << samples_ << ". Sub #" << sub_->config_.subscriber_id << " get data"
//					  << std::endl;
//		}
//	}
//}

template<class T, class TPubSubType, class Cacher>
void ConcreteSubscriber<T, TPubSubType, Cacher>::SubscriberListener::on_data_available(
	eprosima::fastdds::dds::DataReader* reader)
{
	eprosima::fastdds::dds::SampleInfo info;
	auto res = reader->take_next_sample(&data_sample_, &info);
	if (res == ReturnCode_t::RETCODE_OK)
	{
		if (info.valid_data)
		{
			samples_++;
			//TODO по другому надо как то проверять
			{
				if (samples_ >= sub_->config_.samples)
				{
					sub_->stop_ = true;
				}
				auto data_ptr = std::make_shared<T>(data_sample_);
				sub_->on_topic_received_->operator()(std::static_pointer_cast<void>(data_ptr));
				sub_->cacheData(data_sample_);
			}
		}
	}
}

template<class T, class TPubSubType, class Cacher>
void ConcreteSubscriber<T, TPubSubType, Cacher>::SubscriberListener::on_subscription_matched(
	eprosima::fastdds::dds::DataReader* reader,
	const eprosima::fastdds::dds::SubscriptionMatchedStatus& info)
{
	if (info.current_count_change == 1)
	{
		matched_ += info.current_count_change;
		std::cout << "ConcreteSubscriber matched." << std::endl;
	}
	else if (info.current_count_change == -1)
	{
		matched_ += info.current_count_change;
		std::cout << "ConcreteSubscriber unmatched." << std::endl;

		if (info.current_count == 0)
		{
			this->sub_->stop_ = true;
		}
	}
	else
	{
		std::cout << "ConcreteSubscriber: " << info.current_count_change
				  << " is not a valid value for SubscriptionMatchedStatus current count "
					 "change"
				  << std::endl;
	}
}

template<class T, class TPubSubType, class Cacher>
void ConcreteSubscriber<T, TPubSubType, Cacher>::SubscriberListener::on_requested_deadline_missed(
	eprosima::fastdds::dds::DataReader* reader,
	const eprosima::fastdds::dds::RequestedDeadlineMissedStatus& info)
{
	std::cout << "Deadline was missed" << std::endl;
}

template<class T, class TPubSubType, class Cacher>
void ConcreteSubscriber<T, TPubSubType, Cacher>::SubscriberListener::recieveMessageTest()
{
	auto timestamp = TimeConverter::GetTime_LLmcs();
	if (!first_)
	{
		analyser_ = PackageAnalyser::getInstance();
		analyser_->resetStart();
		analyser_->setInitialInfo(sub_->config_.topic_type_name + " "
								  + std::to_string(sub_->config_.vector_size));
		first_ = true;
	}
	analyser_->pushPackageTimestamp({data_sample_.time_service(),
									 timestamp,
									 timestamp - data_sample_.time_service(),
									 T::getCdrSerializedSize(data_sample_)});
	if (info.valid_data)
	{
		samples_++;
		//TODO по другому надо как то проверять
		{
			if (samples_ >= sub_->config_.samples)
			{
				sub_->stop_ = true;
			}
			sub_->cacheData(data_sample_);
		}
	}
}