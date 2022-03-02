#ifndef SUBSCRIBER_FACTORY_H_
#define SUBSCRIBER_FACTORY_H_

#include <deque>
#include <mutex>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include "DataObserver.h"
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
	virtual void* getData() = 0;
protected:
};

template <class T, class TPubSubType>
class ConcreteSubscriber : public AbstractDdsSubscriber
{
public:
	ConcreteSubscriber(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const SubscriberConfig& config,
		DataObserver* observer)
		: participant_(participant)
		, subscriber_(nullptr)
		, reader_(nullptr)
		, topic_(nullptr)
		, config_(config)
		, listener_(this)
		, support_type_(new TPubSubType())
		, observer_(observer)
		, stop_(false)
	{
		setDataSize();
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

		topic_ = participant_->create_topic(config_.topic_name, config_.topic_type_name, eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);
		if (topic_ == nullptr)
		{
			return false;
		}

		subscriber_ = participant_->create_subscriber(eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT, nullptr);
		if (subscriber_ == nullptr)
		{
			return false;
		}

		reader_ = subscriber_->create_datareader(
			topic_,
			eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT,
			&listener_);
		if (reader_ == nullptr)
		{
			return false;
		}

		return true;
	}
	void run() override
	{
		while (!stop_)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(config_.sleep));
		}
	}

	void setConfig(const SubscriberConfig& config) override
	{
		config_ = config;
	}
	void* getData() override
	{
		return &data_;
	}

private:
	// Принимает только данные в этом формате
	std::deque<T> data_;
	T data_sample_;

	DataObserver* observer_;

	bool stop_;

	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Subscriber* subscriber_;
	eprosima::fastdds::dds::DataReader* reader_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::TypeSupport support_type_; // TODO не нужна как поле ?
	SubscriberConfig config_;

	void setDataSize() {};

	void update() {};

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
			eprosima::fastdds::dds::SampleInfo info;
			if (reader->take_next_sample(&sub_->data_sample_, &info) == ReturnCode_t::RETCODE_OK)
			{
				if (info.valid_data)
				{
					samples_++;
					{
						std::lock_guard<std::mutex> guard(std::mutex());
						sub_->data_.push_back(sub_->data_sample_);
					}
					if (samples_ == sub_->config_.samples)
					{
						sub_->update();
						samples_ = 0;
					}
					std::cout << "Sub #" << sub_->config_.subscriber_id << " get data" << std::endl;
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

template<>
void ConcreteSubscriber<DDSData, DDSDataPubSubType>::setDataSize()
{
	std::vector<int> v(10, 0);
	DataCollectionInt dataCollectionInt;
	dataCollectionInt.value(v);
	data_sample_.data_int(dataCollectionInt);
}

template<>
void ConcreteSubscriber<DDSData, DDSDataPubSubType>::update()
{
	std::lock_guard<std::mutex> guard(std::mutex());
	observer_->handleDdsData({ 
		std::make_move_iterator(data_.begin()), 
		std::make_move_iterator(data_.end()) 
	});
}

class SubscriberFactory
{
public:
	virtual ~SubscriberFactory() {}
	AbstractDdsSubscriber* createSubscriber(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const SubscriberConfig& config,
		DataObserver* observer) const;
protected:

};

TopicType string2TopicType(std::string type_name);

std::string TopicType2string(TopicType type);

#endif //!SUBSCRIBER_FACTORY_H_