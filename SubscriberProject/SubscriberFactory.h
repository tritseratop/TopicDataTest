#ifndef SUBSCRIBER_FACTORY_H_
#define SUBSCRIBER_FACTORY_H_

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>

#include "../TypeTopicsDDS/TypeTopicsPubSubTypes.h"

struct SubscriberConfiguration
{
	int16_t subscriber_id = 0;
	uint16_t vector_size = 0;
	std::string topic_name = "";
	std::string topic_type = "";
};

class AbstractDdsSubscriber
{
public:
	virtual ~AbstractDdsSubscriber() {};
	virtual bool init() = 0;
	virtual void run(uint32_t samples) = 0;
protected:
	// eprosima::fastdds::dds::DomainParticipant* participant_;
};

class DDSDataSubscriber : public AbstractDdsSubscriber
{
public:
	DDSDataSubscriber(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const SubscriberConfiguration& config);
	~DDSDataSubscriber() override;

	bool init() override;
	void run(uint32_t samples) override;

private:
	// Принимает только данные в этом формате
	DDSData data_;

	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Subscriber* subscriber_;
	eprosima::fastdds::dds::DataReader* reader_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::TypeSupport support_type_; // TODO не нужна как поле ?
	SubscriberConfiguration config_;

	class DDSDataSubscriberListener : public eprosima::fastdds::dds::DataReaderListener
	{
	public:
		DDSDataSubscriberListener(
			DDSDataSubscriber* subscriber) {};
		~DDSDataSubscriberListener() override {};

		void on_data_available(
			eprosima::fastdds::dds::DataReader* reader) override {};

		void on_subscription_matched(
			eprosima::fastdds::dds::DataReader* reader,
			const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override {};

		int matched_;
		uint32_t samples_; // TODO atomic??
		DDSDataSubscriber* sub_;
	} listener_;
};

class DDSDataExSubscriber : public AbstractDdsSubscriber
{
public:
	DDSDataExSubscriber(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const SubscriberConfiguration& config);
	~DDSDataExSubscriber() override;

	bool init() override;
	void run(uint32_t samples) override;

private:
	// Принимает только данные в этом формате
	DDSDataEx data_;

	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Subscriber* subscriber_;
	eprosima::fastdds::dds::DataReader* reader_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::TypeSupport support_type_; // TODO не нужна как поле ?
	SubscriberConfiguration config_;

	class DDSDataExSubscriberListener : public eprosima::fastdds::dds::DataReaderListener
	{
	public:
		DDSDataExSubscriberListener(
			DDSDataExSubscriber* subscriber) {};
		~DDSDataExSubscriberListener() override {};

		void on_data_available(
			eprosima::fastdds::dds::DataReader* reader) override {};

		void on_subscription_matched(
			eprosima::fastdds::dds::DataReader* reader,
			const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override {};

		int matched_;
		uint32_t samples_; // TODO atomic??
		DDSDataExSubscriber* sub_;
	} listener_;
};

class SubscriberFactory
{
public:
	virtual ~SubscriberFactory() {}
	virtual AbstractDdsSubscriber* createSubscriber(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const SubscriberConfiguration& config) const = 0;
protected:

};

class DDSDataSubscriberCreator : public SubscriberFactory
{
public:
	AbstractDdsSubscriber* createSubscriber(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const SubscriberConfiguration& config) const override;
};

class DDSDataExSubscriberCreator : public SubscriberFactory
{
public:
	AbstractDdsSubscriber* createSubscriber(
		eprosima::fastdds::dds::DomainParticipant* participant,
		const SubscriberConfiguration& config) const override;
};

#endif //!SUBSCRIBER_FACTORY_H_