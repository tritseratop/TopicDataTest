#ifndef PUBLISHER_FACTORY_H_
#define PUBLISHER_FACTORY_H_

#include <thread>
#include <mutex>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include <fastrtps/transport/TCPv4TransportDescriptor.h>

#include "../TypeTopicsDDS/TypeTopicsPubSubTypes.h"

enum TopicType
{
	DDS_DATA, 
	DDS_DATA_EX,
	DDS_ALARM,
	DDS_EX_ALARM,
	UNKNOWN
};

struct PublisherConfig
{
	int16_t subscriber_id = 0;
	uint16_t vector_size = 0;
	std::string topic_name = "";
	std::string topic_type_name = "";
	TopicType topic_type = TopicType::UNKNOWN;

	// listener settings
	uint32_t samples = 10;
	uint32_t sleep = 1000;

	friend bool operator==(const PublisherConfig& lhs, const PublisherConfig& rhs);
};

class AbstractDdsPublisher
{
public:
	virtual ~AbstractDdsPublisher() {};
	virtual bool init() = 0;
	virtual void run() = 0;
	virtual void setData(void* data, size_t size) = 0;
	virtual TopicType getTopicType() = 0;
	//virtual void setConfig(const SubscriberConfig& config) = 0;
protected:
};

template <class T, class TPubSubType>
class ConcretePublisher : public AbstractDdsPublisher
{
public:
	ConcretePublisher(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const PublisherConfig& config
	)
		: participant_(participant)
		, config_(config)
		, publisher_(nullptr)
		, topic_(nullptr)
		, writer_(nullptr)
		, type_(new TPubSubType())
		, listener_(this)
		, topic_type_(config.topic_type)
	{
	}

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
			participant_->delete_topic(topic_);
		}
	}

	void setData(void* data, size_t size) override
	{
		//T* data_p;
		std::memcpy(&data_, data, size);
		//data_ = *(data_p);
	}

	bool init() override
	{
		type_.register_type(participant_);

		topic_ = participant_->create_topic(config_.topic_name, config_.topic_type_name, TOPIC_QOS_DEFAULT);
		if (topic_ == nullptr)
		{
			return false;
		}

		// Create the Publisher
		publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
		if (publisher_ == nullptr)
		{
			return false;
		}

		writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, &listener_);
		if (writer_ == nullptr)
		{
			return false;
		}

		return true;
	}

	void run() override
	{
		uint32_t samples_sent = 0;
		while (samples_sent < config_.samples)
		{
			if (publish(writer_, &listener_, samples_sent))
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

	PublisherConfig config_;

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
		{
		}

		~DDSDataListener() override
		{
		}

		void on_publication_matched(
			eprosima::fastdds::dds::DataWriter* writer,
			const eprosima::fastdds::dds::PublicationMatchedStatus& info) override
		{
			if (info.current_count_change == 1)
			{
				std::cout << "ConcretePublisher matched." << std::endl;
				first_connected_ = true;
			}
			else if (info.current_count_change == -1)
			{
				std::cout << "ConcretePublisher unmatched." << std::endl;
			}
			else
			{
				std::cout << info.current_count_change
					<< " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
			}
		}

		ConcretePublisher* pub_;
		int matched_;
		bool first_connected_;

	} listener_;

	bool publish(eprosima::fastdds::dds::DataWriter* writer, const DDSDataListener* listener, uint32_t samples_sent)
	{
		//std::lock_guard<std::mutex> guard(std::mutex());
		if (listener > 0 && listener->first_connected_)
		{
			if (!writer->write(&data_))
			{
				return false;
			}
			data_.time_service(data_.time_service() + 1);
			return true;
		}
		return false;
	}
};

class PublisherFactory
{
public:
	virtual ~PublisherFactory() {}
	AbstractDdsPublisher* createPublisher(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const PublisherConfig& config) const;
protected:

};

TopicType string2TopicType(std::string type_name);

std::string TopicType2string(TopicType type);

#endif //!PUBLISHER_FACTORY_H_