#ifndef SUBSCRIBER_FACTORY_H_
#define SUBSCRIBER_FACTORY_H_

#include "Lib/DdsService/AbstractSubscriber.h"
#include "Utilities/Dds/TypeTopicsDDS/TypeTopicsPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>

namespace scada_ate::dds::subscriber
{
class Factory
{
public:
	virtual ~Factory() { }
	AbstractSubscriber* createSubscriber(eprosima::fastdds::dds::DomainParticipant* participant,
										 const Configure& config,
										 std::shared_ptr<void> cacher) const;
};

template<class T, class TPubSubType, class Cacher>
class ConcreteSubscriber : public AbstractSubscriber
{
public:
	ConcreteSubscriber(eprosima::fastdds::dds::DomainParticipant* participant,
					   const Configure& config,
					   std::shared_ptr<Cacher> cacher)
		: participant_(participant)
		, subscriber_(nullptr)
		, reader_(nullptr)
		, topic_(nullptr)
		, support_type_(new TPubSubType())
		, config_(config)
		, cacher_(cacher)
		, stop_(false)
		, listener_(this)
	{ }

	~ConcreteSubscriber() override;

	bool init() override;

	void setConfig(const Configure& config) override;

private:
	void cacheData(T data_);

private:
	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Subscriber* subscriber_;
	eprosima::fastdds::dds::DataReader* reader_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::TypeSupport support_type_; // TODO не нужна как поле ?

	Configure config_;

	std::shared_ptr<Cacher> cacher_;

	std::atomic<bool> stop_;

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

	} listener_;
};
} // namespace scada_ate::dds::subscriber

#endif //!SUBSCRIBER_FACTORY_H_