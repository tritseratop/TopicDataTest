#include "TestDdsWsGateway/Helpers/Utilities.h"

#include "DdsWsGateway/Lib/WsService/AppComponent.h"
#include "DdsWsGateway/Lib/WsService/Server.h"
#include "WsClient/WSClient.hpp"

#include "DdsWsGateway/Lib/DdsService/Service.h"
#include "DdsWsGateway/Utilities/Dds/TestUtility.h"
#include "PublisherProject/Service.h"

#include "DdsWsGateway/Lib/Common/DataCacher.h"

#include <gtest/gtest.h>

#include <map>

namespace scada_ate
{

std::shared_ptr<void> getCacher(dds::TopicType topic_type, size_t depth)
{
	if (topic_type == dds::TopicType::DDS_DATA || topic_type == dds::TopicType::DDS_DATA_EX)
	{
		return std::static_pointer_cast<void>(std::make_shared<DataCacher>(depth));
	}
	else if (topic_type == dds::TopicType::DDS_ALARM || topic_type == dds::TopicType::DDS_EX_ALARM)
	{
		return std::static_pointer_cast<void>(std::make_shared<AlarmCacher>(depth));
	}
	else
	{
		// TODO выбрасывать исключение
		return nullptr;
	}
}

std::unordered_map<uint32_t, std::shared_ptr<void>>
getCachers(std::vector<dds::subscriber::Config> configs)
{
	std::unordered_map<dds::subscriber::CacheId, std::shared_ptr<void>> cachers;
	for (const auto& config : configs)
	{
		cachers[config.cache_id] = getCacher(config.topic_type, config.cache_depth);
	}
	return cachers;
}

namespace dds
{

template<class DdsTopic>
std::vector<BeforeTopicSend>
getTestCallbacks(std::multimap<uint32_t, std::vector<DdsTopic>>& data_to_send)
{
	std::vector<BeforeTopicSend> callbacks;
	for (auto& [id, sets] : data_to_send)
	{
		callbacks.push_back([&sets](void* data) {
			auto tmp = sets.back();
			(*static_cast<DdsTopic*>(data)) = tmp;
			sets.pop_back();
		});
	}
	return callbacks;
}

void runDdsService(ParticipantConfig<publisher::Config> pub_config,
				   ParticipantConfig<subscriber::Config> sub_config)
{
	/*publisher::Service* mypub = new publisher::Service(pub_config);

	std::multimap<uint32_t, DdsDataSets> datas_to_send;
	for (const auto& config : pub_config.configs)
	{
		datas_to_send[] = getDdsDataSets(config.samples, 15, 3);
	}
	const auto datas = datas_to_send;

	auto callbacks = getTestCallbacks(datas_to_send);

	mypub->initPublishers();
	auto cachers = getCachers(sub_config.configs);

	subscriber::Service* mysub = new subscriber::Service(sub_config);

	std::thread pub_thread([mypub, mysub, &callbacks]() {
		mypub->testRunPublishers(callbacks);
		mysub->stopSubscribers();
	});

	mysub->initSubscribers(cachers);
	mysub->runSubscribers();

	pub_thread.join();
	delete mypub;
	delete mysub;*/

	/*auto sended_data = mergeAndSortByTimeService(datas);
	auto received_data_tmp = cacher->getDataCacheCopy();
	std::vector<MediateDataDto> received_data{std::make_move_iterator(received_data_tmp.begin()),
											  std::make_move_iterator(received_data_tmp.end())};
	sortByTimeService(received_data);

	EXPECT_EQ(sended_data.size(), received_data.size());

	DdsDataMapper mapper;
	MediateDtoMapper dto_mapper;
	for (size_t i = 0; i < 200; ++i)
	{
		auto sended_sample = dto_mapper.toString(mapper.toMediateDataDto(sended_data[i], info));
		auto received_sample = dto_mapper.toString(received_data[i]);
		EXPECT_EQ(sended_sample, received_sample);
	}*/
}
} // namespace dds

namespace ws
{
void runWsConnection(TestCallback& test_callback, OnMessageRead& on_message_read)
{
	oatpp::base::Environment::init();
	{
		int64_t init_disp = 1'000'000'000'000'000;

		const Config ws_conf;
		auto group = std::make_shared<Group>(0);
		std::unordered_map<int64_t, std::shared_ptr<Group>> groups;
		groups[group->getId()] = group;

		AppComponent component(ws_conf, groups);

		Server server;

		std::thread server_thread([&server]() { server.run(); });

		WSClient wsclient(ws_conf);
		std::thread client_thread(
			[&wsclient, &on_message_read]() { wsclient.run(on_message_read); });

		group->runTestCallback(test_callback);

		client_thread.join();
		server.stop();
		server_thread.join();
	}
	oatpp::base::Environment::destroy();
}

void runWsConnection(TestCallback& test_callback,
					 OnMessageRead& on_message_read,
					 std::shared_ptr<Cacher> cacher)
{
	oatpp::base::Environment::init();
	{
		int64_t init_disp = 1'000'000'000'000'000;

		const Config ws_conf;
		auto group = std::make_shared<Group>(0, cacher);
		std::unordered_map<int64_t, std::shared_ptr<Group>> groups;
		groups[group->getId()] = group;

		AppComponent component(ws_conf, groups);

		Server server;

		std::thread server_thread([&server]() { server.run(); });

		WSClient wsclient(ws_conf);
		std::thread client_thread(
			[&wsclient, &on_message_read]() { wsclient.run(on_message_read); });

		group->runTestCallback(test_callback);

		client_thread.join();
		server.stop();
		server_thread.join();
	}
	oatpp::base::Environment::destroy();
}
} // namespace ws
} // namespace scada_ate
