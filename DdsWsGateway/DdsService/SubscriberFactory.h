#ifndef SUBSCRIBER_FACTORY_H_
#define SUBSCRIBER_FACTORY_H_


#include "../../DdsWsGatewayUtilities/DataObserver.h"
#include "../../DdsWsGatewayUtilities/ThreadSafeQueue/ThreadSafeQueue.h"
#include "../../DdsWsGatewayUtilities/TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "../../DdsWsGatewayUtilities/DdsCommonClasses.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include <mutex>

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
		const SubscriberConfig& config,
		DataObserver* observer)
		: participant_(participant)
		, subscriber_(nullptr)
		, reader_(nullptr)
		, topic_(nullptr)
		, config_(config)
		, support_type_(new TPubSubType())
		, observer_(observer)
		, stop_(false)
		, listener_(this)
	{
		setDataSize();
	}
	~ConcreteSubscriber() override
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
		if (subscriber_ != nullptr)
		{
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

	bool init() override
	{
		stop_ = false;
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

		eprosima::fastdds::dds::DataReaderQos rqos;
		rqos.history().kind = eprosima::fastdds::dds::KEEP_LAST_HISTORY_QOS;
		rqos.history().depth = 30;
		rqos.resource_limits().max_samples = 50;
		rqos.resource_limits().allocated_samples = 20;
		rqos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
		rqos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;
		rqos.deadline().period.nanosec = config_.sleep * 1000;
		reader_ = subscriber_->create_datareader(
			topic_,
			rqos,
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

private:
	T data_sample_;

	DataObserver* observer_;

	std::atomic<bool> stop_;

	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Subscriber* subscriber_;
	eprosima::fastdds::dds::DataReader* reader_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::TypeSupport support_type_; // TODO не нужна как поле ?
	SubscriberConfig config_;

	void setDataSize() {};

	void cacheData(const T& data_) {
		observer_->cache(data_, config_.info);
	}

	void runDataSending();

	class SubscriberListener : public eprosima::fastdds::dds::DataReaderListener
	{
	public:
		SubscriberListener(
			ConcreteSubscriber* subscriber)
			: matched_(0)
			, samples_(0)
			, sub_(subscriber)
		{
		}
		~SubscriberListener() override
		{
		}

		// TODO может ли вызываться в разных потоках?
		void on_data_available(
			eprosima::fastdds::dds::DataReader* reader) override
		{
			eprosima::fastdds::dds::SampleInfo info;
			if (reader->take_next_sample(&data_sample_, &info) == ReturnCode_t::RETCODE_OK)
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
						sub_->cacheData(data_sample_);
					}
					std::cout << samples_ << ". Sub #" << sub_->config_.subscriber_id << " get data" << std::endl;
				}
			}
		}

		void on_subscription_matched(
			eprosima::fastdds::dds::DataReader* reader,
			const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override
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
					<< " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
			}
		}

		void on_requested_deadline_missed(
			eprosima::fastdds::dds::DataReader* reader,
			const eprosima::fastdds::dds::RequestedDeadlineMissedStatus& info) override
		{
			std::cout << "Deadline was missed" << std::endl;
		}

		int matched_;
		uint32_t samples_; // TODO atomic??
		T data_sample_;
		ConcreteSubscriber* sub_;
	} listener_;
};

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

#endif //!SUBSCRIBER_FACTORY_H_