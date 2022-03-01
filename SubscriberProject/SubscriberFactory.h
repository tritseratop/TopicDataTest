#ifndef SUBSCRIBER_FACTORY_H_
#define SUBSCRIBER_FACTORY_H_

#include <deque>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include "../TypeTopicsDDS/TypeTopicsPubSubTypes.h"

enum TopicType
{
	DDS_DATA, 
	DDS_DATA_EX,
	DDS_ALARM,
	DDS_EX_ALARM,
	UNKNOWN
};

struct SubscriberConfig
{
	int16_t subscriber_id = 0;
	uint16_t vector_size = 0;
	std::string topic_name = "";
	std::string topic_type_name = "";
	TopicType topic_type = TopicType::UNKNOWN;

	// listener settings
	uint32_t samples = 10;
	uint32_t sleep = 1000;

	friend bool operator==(const SubscriberConfig& lhs, const SubscriberConfig& rhs);
};

class AbstractDdsSubscriber
{
public:
	virtual ~AbstractDdsSubscriber() {};
	virtual bool init() = 0;
	virtual void run() = 0;
	virtual void setConfig(const SubscriberConfig& config) = 0;
protected:
};

template <class T, class TPubSubType>
class ConcreteSubscriber : public AbstractDdsSubscriber
{
public:
	ConcreteSubscriber(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const SubscriberConfig& config)
		: participant_(participant)
		, subscriber_(nullptr)
		, reader_(nullptr)
		, topic_(nullptr)
		, config_(config)
		, listener_(this)
		, support_type_(new TPubSubType())
	{
	}
	~ConcreteSubscriber() override
	{
		if (reader_ != nullptr)
		{
			subscriber_->delete_datareader(reader_);
		}
		if (subscriber_ != nullptr)
		{
			participant_->delete_subscriber(subscriber_);
		}
		if (topic_ != nullptr)
		{
			participant_->delete_topic(topic_);
		}
	}

	bool init() override
	{
		if (participant_ == nullptr)
		{
			return false;
		}

		support_type_.register_type(participant_);

		topic_ = participant_->create_topic(config_.topic_name, config_.topic_type_name, TOPIC_QOS_DEFAULT);
		if (topic_ == nullptr)
		{
			return false;
		}

		subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
		if (subscriber_ == nullptr)
		{
			return false;
		}

		reader_ = subscriber_->create_datareader(
			topic_,
			DATAREADER_QOS_DEFAULT,
			&listener_);
		if (reader_ == nullptr)
		{
			return false;
		}

		return true;
	}
	void run() override
	{
		while (listener_.samples_ < config_.samples)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(config_.sleep));
		}
	}

	void setConfig(const SubscriberConfig& config) override
	{
		config_ = config;
	}

private:
	// Принимает только данные в этом формате
	std::deque<T> data_;

	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Subscriber* subscriber_;
	eprosima::fastdds::dds::DataReader* reader_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::TypeSupport support_type_; // TODO не нужна как поле ?
	SubscriberConfig config_;

	class DDSDataSubscriberListener : public eprosima::fastdds::dds::DataReaderListener
	{
	public:
		DDSDataSubscriberListener(
			ConcreteSubscriber* subscriber)
			: matched_(0)
			, samples_(0)
			, sub_(subscriber)
		{
		}
		~DDSDataSubscriberListener() override
		{
		}

		void on_data_available(
			eprosima::fastdds::dds::DataReader* reader) override
		{
			SampleInfo info;
			if (reader->take_next_sample(&data_, &info) == ReturnCode_t::RETCODE_OK)
			{
				if (info.valid_data)
				{
					samples_++;
					sub_->data_.push_back(data_);
				}
			}
		}

		void on_subscription_matched(
			eprosima::fastdds::dds::DataReader* reader,
			const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override
		{
			if (info.current_count_change == 1)
			{
				std::cout << "ConcreteSubscriber matched." << std::endl;
			}
			else if (info.current_count_change == -1)
			{
				std::cout << "ConcreteSubscriber unmatched." << std::endl;
			}
			else
			{
				std::cout << "ConcreteSubscriber: " << info.current_count_change
					<< " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
			}
		}

		T data_;
		int matched_;
		uint32_t samples_; // TODO atomic??
		ConcreteSubscriber* sub_;
	} listener_;
};

class SubscriberFactory
{
public:
	virtual ~SubscriberFactory() {}
	AbstractDdsSubscriber* createSubscriber(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const SubscriberConfig& config) const;
protected:

};

TopicType string2TopicType(std::string type_name);

std::string TopicType2string(TopicType type);

#endif //!SUBSCRIBER_FACTORY_H_