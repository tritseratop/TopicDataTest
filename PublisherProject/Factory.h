#ifndef PUBLISHER_FACTORY_H_
#define PUBLISHER_FACTORY_H_

#include "PublisherProject/Config.h"

#include "DdsWsGateway/Utilities/Common/TimeConverter/TimeConverter.hpp"
#include "DdsWsGateway/Utilities/Dds/Callbacks.h"
#include "DdsWsGateway/Utilities/Dds/TypeTopicsDDS/TypeTopicsPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>

#include <fastrtps/transport/TCPv4TransportDescriptor.h>

#include <mutex>
#include <thread>

namespace scada_ate::dds::publisher
{
using eprosima::fastrtps::types::ReturnCode_t;

class AbstractDdsPublisher
{
public:
	virtual ~AbstractDdsPublisher(){};
	virtual bool init() = 0;
	virtual void run() = 0;

	virtual void testRun(BeforeTopicSend&) = 0;
	virtual void testRun(BeforeTopicSendData&) = 0;

	virtual void setData() = 0;
	virtual void setData(void* data) = 0;
	virtual TopicType getTopicType() = 0;
	//virtual void setConfig(const Config& config) = 0;
protected:
};

template<class T, class TPubSubType>
class ConcretePublisher : public AbstractDdsPublisher
{
public:
	ConcretePublisher(eprosima::fastdds::dds::DomainParticipant* participant, const Config& config)
		: participant_(participant)
		, config_(config)
		, publisher_(nullptr)
		, topic_(nullptr)
		, writer_(nullptr)
		, type_(new TPubSubType())
		, listener_(this)
		, topic_type_(config.topic_type)
		, stop_(false)
	{ }

	~ConcretePublisher() override
	{
		if (writer_ != nullptr)
		{
			publisher_->delete_datawriter(writer_);
		}
		if (publisher_ != nullptr)
		{
			participant_->delete_publisher(publisher_);
		}
		if (topic_ != nullptr)
		{
			auto res = participant_->delete_topic(topic_);
			if (res != ReturnCode_t::RETCODE_OK)
			{
				std::cout << res() << std::endl;
			}
		}
	}

	void setData(void* data) override
	{
		//T* data_p;
		data_ = *(static_cast<T*>(data));
		//std::memcpy(&data_, data, size);
		//data_ = *(data_p);
	}

	void setData() override;

	bool init() override
	{
		type_.register_type(participant_);

		topic_ = participant_->create_topic(
			config_.topic_name, config_.topic_type_name, eprosima::fastdds::dds::TOPIC_QOS_DEFAULT);
		if (topic_ == nullptr)
		{
			return false;
		}

		// Create the Publisher
		publisher_ = participant_->create_publisher(eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT,
													nullptr);
		if (publisher_ == nullptr)
		{
			return false;
		}

		eprosima::fastdds::dds::DataWriterQos wqos;
		wqos.history().kind = eprosima::fastdds::dds::KEEP_LAST_HISTORY_QOS;
		wqos.history().depth = 30;
		wqos.resource_limits().max_samples = 50;
		wqos.resource_limits().allocated_samples = 20;
		wqos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;
		wqos.durability().kind = eprosima::fastdds::dds::TRANSIENT_LOCAL_DURABILITY_QOS;
		wqos.deadline().period.nanosec = config_.sleep * 1000;

		if (config_.isSync)
		{
			wqos.publish_mode().kind = eprosima::fastdds::dds::SYNCHRONOUS_PUBLISH_MODE;
		}

		writer_ = publisher_->create_datawriter(topic_, wqos, &listener_);
		if (writer_ == nullptr)
		{
			return false;
		}

		return true;
	}

	void run() override
	{
		uint32_t samples_sent = 0;
		while (!stop_ && samples_sent < config_.samples)
		{
			if (publish(writer_, &listener_, samples_sent))
			{
				samples_sent++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config_.sleep));
		}
	}

	void testRun(BeforeTopicSend& before_topic_send) override
	{
		uint32_t samples_sent = 0;
		while (!stop_ && samples_sent < config_.samples)
		{
			if (testPublish(before_topic_send, &listener_))
			{
				samples_sent++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config_.sleep));
		}
	}

	void testRun(BeforeTopicSendData& before_topic_send) override
	{
		uint32_t samples_sent = 0;
		while (!stop_ && samples_sent < config_.samples)
		{
			if (testPublish(before_topic_send, writer_, &listener_))
			{
				samples_sent++;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(config_.sleep));
		}
	}

	TopicType getTopicType() override
	{
		return topic_type_;
	}

private:
	// Принимает только данные в этом формате
	T data_;
	const TopicType topic_type_;
	bool stop_;

	Config config_;

	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Publisher* publisher_;
	eprosima::fastdds::dds::DataWriter* writer_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::TypeSupport type_; // TODO не нужна как поле ?

	class DDSDataListener : public eprosima::fastdds::dds::DataWriterListener
	{
	public:
		DDSDataListener(ConcretePublisher* publisher)
			: matched_(0)
			, first_connected_(false)
			, pub_(publisher)
		{ }

		~DDSDataListener() override { }

		void on_publication_matched(
			eprosima::fastdds::dds::DataWriter* writer,
			const eprosima::fastdds::dds::PublicationMatchedStatus& info) override
		{
			if (info.current_count_change == 1)
			{
				matched_ += info.current_count_change;
				std::cout << "ConcretePublisher matched." << std::endl;
				first_connected_ = true;
			}
			else if (info.current_count_change == -1)
			{
				matched_ += info.current_count_change;
				std::cout << "ConcretePublisher unmatched." << std::endl;
				if (matched_ == 0)
				{
					this->pub_->stop_ = true;
				}
			}
			else
			{
				std::cout << info.current_count_change
						  << " is not a valid value for PublicationMatchedStatus current count "
							 "change"
						  << std::endl;
			}
		}

		ConcretePublisher* pub_;
		int matched_;
		bool first_connected_;

	} listener_;

	bool publish(eprosima::fastdds::dds::DataWriter* writer,
				 const DDSDataListener* listener,
				 uint32_t samples_sent)
	{
		//std::lock_guard<std::mutex> guard(std::mutex());
		if (listener != nullptr && listener->first_connected_)
		{
			data_.time_service(TimeConverter::GetTime_LLmcs());
			if (!writer->write(&data_))
			{
				return false;
			}
			return true;
		}
		return false;
	}

	bool testPublish(BeforeTopicSend& before_topic_send, const DDSDataListener* listener)
	{
		//std::lock_guard<std::mutex> guard(std::mutex());
		if (listener != nullptr && listener->first_connected_)
		{
			return before_topic_send(writer_);
		}
		return false;
	}

	bool testPublish(BeforeTopicSendData& before_topic_send,
					 eprosima::fastdds::dds::DataWriter* writer,
					 const DDSDataListener* listener)
	{
		//std::lock_guard<std::mutex> guard(std::mutex());
		if (listener != nullptr && listener->first_connected_)
		{
			before_topic_send(&data_);
			if (!writer->write(&data_))
			{
				return false;
			}
			return true;
		}
		return false;
	}
};

class Factory
{
public:
	virtual ~Factory() { }
	AbstractDdsPublisher* createPublisher(eprosima::fastdds::dds::DomainParticipant* participant,
										  const Config& config) const;

protected:
};
} // namespace scada_ate::dds::publisher

#endif //!PUBLISHER_FACTORY_H_