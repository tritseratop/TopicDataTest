#ifndef SUBSCRIBER_FACTORY_H_
#define SUBSCRIBER_FACTORY_H_

#include "Lib/Common/Notifier.h"
#include "Utilities/DdsCommonClasses.h"
#include "Utilities/PackageAnalyser.h"
#include "Utilities/ThreadSafeQueue/ThreadSafeQueue.h"
#include "Utilities/TypeTopicsDDS/TypeTopicsPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>

#include <mutex>

class AbstractDdsSubscriber
{
public:
	virtual ~AbstractDdsSubscriber(){};
	virtual bool init() = 0;
	virtual void run() = 0;
	virtual void stop() = 0;
	virtual void setConfig(const SubscriberConfig& config) = 0;
};

template<class T, class TPubSubType, class Cacher>
class ConcreteSubscriber : public AbstractDdsSubscriber
{
public:
	ConcreteSubscriber(eprosima::fastdds::dds::DomainParticipant* participant,
					   const SubscriberConfig& config,
					   std::shared_ptr<Cacher> cacher,
					   std::shared_ptr<OnTopicReceived> on_topic_received);

	~ConcreteSubscriber() override;

	bool init() override;

	void run() override;

	void stop() override;

	void setConfig(const SubscriberConfig& config) override;

private:
	void cacheData(T data_);

	void setTestCallback(OnTopicReceived on_topic_received);

private:
	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Subscriber* subscriber_;
	eprosima::fastdds::dds::DataReader* reader_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::TypeSupport support_type_; // TODO не нужна как поле ?

	SubscriberConfig config_;

	std::shared_ptr<Cacher> cacher_;

	std::atomic<bool> stop_;

	std::shared_ptr<OnTopicReceived> on_topic_received_;

	class SubscriberListener : public eprosima::fastdds::dds::DataReaderListener
	{
	public:
		SubscriberListener(ConcreteSubscriber* subscriber);

		~SubscriberListener() override;

		//void on_data_available(eprosima::fastdds::dds::DataReader* reader) override;

		void on_data_available(eprosima::fastdds::dds::DataReader* reader) override;

		void on_subscription_matched(
			eprosima::fastdds::dds::DataReader* reader,
			const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;

		void on_requested_deadline_missed(
			eprosima::fastdds::dds::DataReader* reader,
			const eprosima::fastdds::dds::RequestedDeadlineMissedStatus& info) override;

	private:
		void recieveMessageTest();

	private:
		ConcreteSubscriber* sub_;
		T data_sample_;

		uint32_t samples_; // TODO atomic??
		int matched_;
		bool first_;

		PackageAnalyser* analyser_;
	} listener_;
};

class SubscriberFactory
{
public:
	virtual ~SubscriberFactory() { }
	AbstractDdsSubscriber*
	createSubscriber(eprosima::fastdds::dds::DomainParticipant* participant,
					 const SubscriberConfig& config,
					 std::shared_ptr<void> cacher,
					 std::shared_ptr<OnTopicReceived> on_topic_received) const;

protected:
};

#endif //!SUBSCRIBER_FACTORY_H_