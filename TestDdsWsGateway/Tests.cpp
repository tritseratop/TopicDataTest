#include "TestDdsWsGateway/Helpers/Utilities.h"

#include "PublisherProject/Service.h"

#include "DdsWsGatewayService/Lib/Common/Notifier.h"
#include "DdsWsGatewayService/Lib/DdsService/Service.h"
#include "DdsWsGatewayService/Lib/WsService/Group.h"
#include "DdsWsGatewayService/Utilities/Common/PackageAnalyser.h"
#include "DdsWsGatewayService/Utilities/Common/nlohmann/json.hpp"
#include "DdsWsGatewayService/Utilities/Dds/TestUtility.h"
#include "DdsWsGatewayService/Utilities/Ws/TestUtility.h"

#include "oatpp/parser/json/Beautifier.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include <gtest/gtest.h>

namespace scada_ate
{
std::vector<dds::subscriber::Configure> configs({
	{0, 100, "DDSData1", "DDSData", dds::TopicType::DDS_DATA, 100, 100},
	//        {1, 10000, "DDSData2", "DDSData", TopicType::DDS_DATA, 100, 100},
	//        {2, 10000, "DDSData3", "DDSData", TopicType::DDS_DATA, 100, 100},
});

dds::ServiceConfigForTest<dds::subscriber::Configure> config({"Participant_sub",
															  dds::Transport::TCP,
															  "127.0.0.1",
															  4042,
															  {"127.0.0.1"},
															  configs,
															  1000,
															  1000,
															  1000,
															  1000,
															  1000,
															  1000,
															  1000,
															  1000,
															  1000,
															  1000,
															  1000,
															  1000});

TEST(HelloTest, BasicAssertions)
{
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
}

TEST(DdsToDtoMapping, DdsDataToMediateDto)
{
	using namespace scada_ate::dds;
	{
		auto dds_data = getEqualDdsData(4, 1);

		DdsDataMapper ddsdata_mapper;
		MediateDataDto d = ddsdata_mapper.toMediateDataDto(dds_data.data, dds_data.tags_info);
		EXPECT_EQ(dds_data.dto, d);
	}
	{
		auto dds_data = getEqualDdsData(5, 8);

		DdsDataMapper ddsdata_mapper;
		MediateDataDto d = ddsdata_mapper.toMediateDataDto(dds_data.data, dds_data.tags_info);
		EXPECT_EQ(dds_data.dto, d);
	}
	{
		auto dds_data = getEqualDdsData(0, 8);

		DdsDataMapper ddsdata_mapper;
		MediateDataDto d = ddsdata_mapper.toMediateDataDto(dds_data.data, dds_data.tags_info);
		EXPECT_EQ(dds_data.dto, d);
	}
}

TEST(DdsToDtoMapping, DdsDataExToMediateDto)
{
	using namespace scada_ate::dds;
	auto vectors = VectorsForData(4, 1);

	DdsDataExMapper ddsdataex_mapper;
	{
		DdsDataExUnion data_ex_union = getEqualDdsDataEx(3, 5);
		MediateDataDto dto = ddsdataex_mapper.toMediateDataDto(
			data_ex_union.data_ex, data_ex_union.tags_info, data_ex_union.dto_to_change);
		EXPECT_EQ(dto, data_ex_union.result_dto);
	}
	{
		DdsDataExUnion data_ex_union = getEqualDdsDataEx(3, 0);
		MediateDataDto dto = ddsdataex_mapper.toMediateDataDto(
			data_ex_union.data_ex, data_ex_union.tags_info, data_ex_union.dto_to_change);
		EXPECT_EQ(dto, data_ex_union.result_dto);
	}
	{
		DdsDataExUnion data_ex_union = getEqualDdsDataEx(500, 1000);
		MediateDataDto dto = ddsdataex_mapper.toMediateDataDto(
			data_ex_union.data_ex, data_ex_union.tags_info, data_ex_union.dto_to_change);
		EXPECT_EQ(dto, data_ex_union.result_dto);
	}
	{
		DdsDataExUnion data_ex_union = getEqualDdsDataEx(0, 0);
		MediateDataDto dto = ddsdataex_mapper.toMediateDataDto(
			data_ex_union.data_ex, data_ex_union.tags_info, data_ex_union.dto_to_change);
		EXPECT_EQ(dto, data_ex_union.result_dto);
	}
	{
		DdsDataExUnion data_ex_union = getEqualDdsDataEx(0, 3);
		MediateDataDto dto = ddsdataex_mapper.toMediateDataDto(
			data_ex_union.data_ex, data_ex_union.tags_info, data_ex_union.dto_to_change);
		EXPECT_EQ(dto, data_ex_union.result_dto);
	}
}

TEST(DtoToWsMapping, MediateDtoToString)
{
	MediateDtoMapper mapper;
	auto data1 = dds::getMediateDataDto(1, 1);
	EXPECT_NE(mapper.toString(data1), mapper.toStringWithCharVectors(data1));
}

TEST(JsonParsing, JsonToTestConditionParsing)
{
	using namespace scada_ate::dds;
	std::istringstream is("{\n"
						  "\"conditions\": [\n"
						  "    {\n"
						  "        \"all_vectors_sizes\": 10,\n"
						  "        \"char_vector_sizes\": 10,\n"
						  "        \"publication_interval\": 100\n"
						  "    },\n"
						  "    {\n"
						  "        \"all_vectors_sizes\": 100,\n"
						  "        \"char_vector_sizes\": 10,\n"
						  "        \"publication_interval\": 100\n"
						  "    },\n"
						  "    {\n"
						  "        \"all_vectors_sizes\": 500,\n"
						  "        \"char_vector_sizes\": 10,\n"
						  "        \"publication_interval\": 100\n"
						  "    },\n"
						  "    {\n"
						  "        \"all_vectors_sizes\": 1000,\n"
						  "        \"char_vector_sizes\": 10,\n"
						  "        \"publication_interval\": 100\n"
						  "    }\n"
						  "],\n"
						  "\"ip\": \"192.168.0.185\",\n"
						  "\"isWsServerRun\": false,\n"
						  "\"isDdsServerRun\": false,\n"
						  "\"isSync\": false,\n"
						  "\"samples_number\": 5\n"
						  "}");
	nlohmann::json json;
	is >> json;
	auto result = parseJsonToGlobalTestConditions(json);
	GlobalTestConditions expected{{
									  {10, 10, 100},
									  {100, 10, 100},
									  {500, 10, 100},
									  {1000, 10, 100},
								  },
								  "192.168.0.185",
								  false,
								  false,
								  false,
								  5};

	EXPECT_EQ(result, expected);

	expected.isSync = true;
	EXPECT_FALSE(result == expected);

	expected.isSync = false;
	expected.isDdsServerRun = true;
	EXPECT_FALSE(result == expected);

	expected.isDdsServerRun = false;
	expected.isWsServerRun = true;
	EXPECT_FALSE(result == expected);
}

TEST(JsonParsing, WriteSizes)
{
	using namespace scada_ate::dds;
	std::ifstream ifile("test_char_size.json");
	nlohmann::json json;
	ifile >> json;
	auto result = parseJsonToGlobalTestConditions(json);

	for (const auto& c : result.conditions)
	{
		auto dds_data = getDdsData(c.all_vectors_sizes, c.char_vector_sizes);
		/*std::cout << "vectors size: " << c.all_vectors_sizes << "\t | "
				  << DDSData::getCdrSerializedSize(dds_data) << std::endl;*/
	}

	for (const auto& c : result.conditions)
	{
		auto dds_data_ex = getDdsDataEx(c.all_vectors_sizes, c.char_vector_sizes);
		/*std::cout << "vectors size: " << c.all_vectors_sizes << "\t | "
				  << DDSDataEx::getCdrSerializedSize(dds_data_ex) << std::endl;*/
	}
}

TEST(setTimeToJsonTest, replace) { }

TEST(setTimeToJsonTest, insert) { }

TEST(WsConnectionTest, RunWithoutCoroutine)
{
	using namespace scada_ate::ws;
	int64_t init_disp = 1'000'000'000'000'000;

	size_t packet_number = 100;
	std::vector<TestPacket> test_packets;
	test_packets.reserve(packet_number);
	std::string message(10000, 'a');

	for (int64_t i = packet_number - 1; i >= 0; --i)
	{
		test_packets.push_back(TestPacket{init_disp + i, message});
	}

	ThreadSafeDeque<int64_t> client_cache;
	OnMessageRead on_message_read = [&client_cache](const oatpp::String& message) {
		client_cache.push_back(getTimeFromJsonString(message));
	};

	ThreadSafeDeque<int64_t> server_cache;
	BeforeMessageSend before_msg_send = [&server_cache](const oatpp::String& message) {
		auto result = getTimeFromJsonString(message);
		server_cache.push_back(result);
	};
	TestCallback test_callback = [&test_packets, &before_msg_send](Group& group) {
		while (!group.hasClients())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		while (!test_packets.empty())
		{
			auto message = nlohmann::json(test_packets.back()).dump();
			test_packets.pop_back();
			group.sendTextMessageToAllAsync(oatpp::String(message), before_msg_send);
		}
		group.sendCloseToAllAsync();
	};

	runWsConnection(test_callback, on_message_read);

	EXPECT_EQ(packet_number, server_cache.size());
	if (server_cache.size() < packet_number)
		return;
	for (int i = 0; i < packet_number; ++i)
	{
		auto server_disp = server_cache.front();

		EXPECT_EQ(init_disp + i, server_disp);
		server_cache.pop_front();
	}

	EXPECT_EQ(packet_number, client_cache.size());
	if (client_cache.size() < packet_number)
		return;
	for (int i = 0; i < packet_number; ++i)
	{
		auto disp = client_cache.front();

		EXPECT_EQ(init_disp + i, disp);
		client_cache.pop_front();
	}
}

void DdsDataTest()
{
	using namespace scada_ate::dds;
	ServiceConfigForTest<publisher::Configure> pub_config({"Participant_pub",
														   Transport::TCP,
														   "127.0.0.1",
														   4043,
														   {"127.0.0.1"},
														   {},
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000});
	publisher::Configure ddsdata_config = {
		0, 10, 10, "DDSData", "DDSData", TopicType::DDS_DATA, 100, 2, false};
	pub_config.configs = {ddsdata_config};

	publisher::Service* mypub = new publisher::Service(pub_config);

	std::vector<DDSData> dds_datas;
	dds_datas.reserve(100);

	for (size_t i = 0; i < 100; ++i)
	{
		dds_datas.push_back(getDdsData(5, 3));
	}

	auto dds_data_tmp = dds_datas;
	auto data = dds_data_tmp.back();

	BeforeTopicSend before_topic_send = [&dds_data_tmp,
										 &data](eprosima::fastdds::dds::DataWriter* writer) {
		//data_.time_service(TimeConverter::GetTime_LLmcs());
		data = dds_data_tmp.back();
		dds_data_tmp.pop_back();
		if (!writer->write(&data))
		{
			return false;
		}
		return true;
	};

	BeforeTopicSendData before_topic_send_data = [&dds_data_tmp](void* data) {
		//data_.time_service(TimeConverter::GetTime_LLmcs());
		auto tmp = dds_data_tmp.back();
		(*static_cast<DDSData*>(data)) = tmp;
		dds_data_tmp.pop_back();
	};

	mypub->initPublishers();

	ServiceConfigForTest<subscriber::Configure> sub_config({"Participant_sub",
															Transport::TCP,
															"127.0.0.1",
															4043,
															{"127.0.0.1"},
															{},
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000});
	AdditionalTopicInfo info = getAdditionalTopicInfo(100);
	subscriber::Configure sub_ddsdata_config = {
		0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, 100, 100, info, true};
	sub_config.configs = {sub_ddsdata_config};

	auto cacher = std::make_shared<DataCacher>(100, info);
	subscriber::Service* mysub = new subscriber::Service(sub_config, cacher);

	std::thread pub_thread([mypub, mysub, &before_topic_send]() {
		mypub->testRunPublishers(before_topic_send);
		mysub->stopSubscribers();
	});

	mysub->initSubscribers();
	mysub->runSubscribers();

	pub_thread.join();
	delete mypub;
	delete mysub;

	auto sub_datas = cacher->getDataCacheCopy();
	EXPECT_EQ(dds_datas.size(), sub_datas.size());
	if (dds_datas.size() != sub_datas.size())
		return;

	DdsDataMapper mapper;
	MediateDtoMapper dto_mapper;
	for (size_t i = 0; i < 100; ++i)
	{
		auto sended_data = dto_mapper.toString(
			mapper.toMediateDataDto(dds_datas[100 - i - 1], info));
		auto received_data = dto_mapper.toString(sub_datas[i]);
		EXPECT_EQ(sended_data, received_data);
	}
}

void DdsDataTestManyTopics()
{
	using namespace scada_ate::dds;
	ServiceConfigForTest<publisher::Configure> pub_config({"Participant_pub",
														   Transport::TCP,
														   "127.0.0.1",
														   4043,
														   {"127.0.0.1"},
														   {},
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000});
	publisher::Configure ddsdata_config1 = {
		0, 10, 10, "DDSData", "DDSData", TopicType::DDS_DATA, 100, 2, false};
	publisher::Configure ddsdata_config2 = {
		0, 100, 10, "DDSData_Second", "DDSData", TopicType::DDS_DATA, 100, 8, false};
	pub_config.configs = {ddsdata_config1, ddsdata_config2};

	publisher::Service* mypub = new publisher::Service(pub_config);

	std::vector<std::vector<DDSData>> dds_datas(2);

	size_t digit = 0;
	for (auto& dds_data : dds_datas)
	{
		++digit;
		for (size_t i = 0; i < 100; ++i)
		{
			dds_data.push_back(getDdsData(5, 3));
			dds_data.back().time_service() = digit * 1000 + i;
		}
	}

	auto dds_data_tmp = dds_datas;
	auto data = dds_data_tmp.back();

	BeforeTopicSendData before_topic_send_data1 = [&dds_data_tmp](void* data) {
		auto tmp = dds_data_tmp[0].back();
		(*static_cast<DDSData*>(data)) = tmp;
		dds_data_tmp[0].pop_back();
	};

	BeforeTopicSendData before_topic_send_data2 = [&dds_data_tmp](void* data) {
		auto tmp = dds_data_tmp[1].back();
		(*static_cast<DDSData*>(data)) = tmp;
		dds_data_tmp[1].pop_back();
	};

	std::vector<BeforeTopicSendData> pub_callbacks{before_topic_send_data1,
												   before_topic_send_data2};

	mypub->initPublishers();

	ServiceConfigForTest<subscriber::Configure> sub_config({"Participant_sub",
															Transport::TCP,
															"127.0.0.1",
															4043,
															{"127.0.0.1"},
															{},
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000});
	AdditionalTopicInfo info = getAdditionalTopicInfo(100);
	subscriber::Configure sub_ddsdata_config = {
		0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, 100, 100, info, true};
	subscriber::Configure sub_ddsdata_config2 = {
		0, 100, "DDSData_Second", "DDSData", TopicType::DDS_DATA, 100, 100, info, true};
	sub_config.configs = {sub_ddsdata_config, sub_ddsdata_config2};

	auto cacher = std::make_shared<DataCacher>(200, info);
	subscriber::Service* mysub = new subscriber::Service(sub_config, cacher);

	std::thread pub_thread([mypub, mysub, &pub_callbacks]() {
		mypub->testRunPublishers(pub_callbacks);
		mysub->stopSubscribers();
	});

	mysub->initSubscribers();
	mysub->runSubscribers();

	pub_thread.join();
	delete mypub;
	delete mysub;

	auto sended_data = std::move(dds_datas[0]);
	std::copy(dds_datas[1].begin(), dds_datas[1].end(), std::back_inserter(sended_data));

	std::sort(sended_data.begin(), sended_data.end(), [](DDSData a, DDSData b) {
		return a.time_service() < b.time_service();
	});

	auto received_data = cacher->getDataCacheCopy();
	std::sort(received_data.begin(), received_data.end(), [](MediateDataDto a, MediateDataDto b) {
		return a.time_service < b.time_service;
	});

	EXPECT_EQ(sended_data.size(), received_data.size());

	DdsDataMapper mapper;
	MediateDtoMapper dto_mapper;
	for (size_t i = 0; i < 200; ++i)
	{
		auto sended_sample = dto_mapper.toString(mapper.toMediateDataDto(sended_data[i], info));
		auto received_sample = dto_mapper.toString(received_data[i]);
		EXPECT_EQ(sended_sample, received_sample);
	}
}

void DdsDataExTest()
{
	using namespace scada_ate::dds;
	ServiceConfigForTest<publisher::Configure> pub_config({"Participant_pub",
														   Transport::TCP,
														   "127.0.0.1",
														   4044,
														   {"127.0.0.1"},
														   {},
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000});
	publisher::Configure ddsdata_config = {
		1, 10, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, 100, 2, false};
	pub_config.configs = {ddsdata_config};

	publisher::Service* mypub = new publisher::Service(pub_config);

	std::vector<DDSDataEx> dds_datas;
	dds_datas.reserve(100);
	AdditionalTopicInfo info = getAdditionalTopicInfo(100);

	for (size_t i = 0; i < 100; ++i)
	{
		auto vectors = VectorsForData(5, 3);
		dds_datas.push_back(getDdsDataEx(vectors, info));
	}

	auto dds_data_tmp = dds_datas;
	auto data = dds_data_tmp.back();

	BeforeTopicSend before_topic_send = [&dds_data_tmp,
										 &data](eprosima::fastdds::dds::DataWriter* writer) {
		//data_.time_service(TimeConverter::GetTime_LLmcs());
		data = dds_data_tmp.back();
		dds_data_tmp.pop_back();
		if (!writer->write(&data))
		{
			return false;
		}
		return true;
	};

	BeforeTopicSendData before_topic_send_data = [&dds_data_tmp](void* data) {
		//data_.time_service(TimeConverter::GetTime_LLmcs());
		auto tmp = dds_data_tmp.back();
		(*static_cast<DDSDataEx*>(data)) = tmp;
		dds_data_tmp.pop_back();
	};

	mypub->initPublishers();

	ServiceConfigForTest<subscriber::Configure> sub_config({"Participant_sub",
															Transport::TCP,
															"127.0.0.1",
															4044,
															{"127.0.0.1"},
															{},
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000});

	subscriber::Configure sub_ddsdata_config = {
		1, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, 100, 100, info, true};
	sub_config.configs = {sub_ddsdata_config};

	auto cacher = std::make_shared<DataCacher>(100, info);
	subscriber::Service* mysub = new subscriber::Service(sub_config, cacher);

	std::thread pub_thread([mypub, mysub, &before_topic_send]() {
		mypub->testRunPublishers(before_topic_send);
		mysub->stopSubscribers();
	});

	mysub->initSubscribers();
	mysub->runSubscribers();

	pub_thread.join();
	delete mypub;
	delete mysub;

	auto sub_datas = cacher->getDataCacheCopy();
	EXPECT_EQ(dds_datas.size(), sub_datas.size());
	if (dds_datas.size() != sub_datas.size())
		return;

	DdsDataExMapper mapper;
	MediateDtoMapper dto_mapper;
	for (size_t i = 0; i < 100; ++i)
	{
		auto sended_data = dto_mapper.toString(
			mapper.toMediateDataDto(dds_datas[100 - i - 1], info));
		auto received_data = dto_mapper.toString(sub_datas[i]);
		EXPECT_EQ(sended_data, received_data);
	}
}

//TEST(DdsConnectionTest, DdsDataExTest)
//{
//	DdsDataExTest();
//}

TEST(DdsConnectionTest, DdsDataTest)
{
	DdsDataTest();
}

TEST(DdsConnectionTest, DdsDataTestManyTopics)
{
	DdsDataTestManyTopics();
}

void OneToOneConnection()
{
	using namespace scada_ate::dds;
	ServiceConfigForTest<publisher::Configure> pub_config({"Participant_pub",
														   Transport::TCP,
														   "127.0.0.1",
														   4043,
														   {"127.0.0.1"},
														   {},
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000,
														   10000});
	publisher::Configure ddsdata_config = {
		0, 10, 10, "DDSData", "DDSData", TopicType::DDS_DATA, 100, 100, false};
	pub_config.configs = {ddsdata_config};

	publisher::Service* mypub = new publisher::Service(pub_config);

	std::vector<DDSData> dds_datas;
	dds_datas.reserve(100);

	for (size_t i = 0; i < 100; ++i)
	{
		dds_datas.push_back(getDdsData(5, 3));
	}

	auto dds_data_tmp = dds_datas;
	auto data = dds_data_tmp.back();

	BeforeTopicSend before_topic_send = [&dds_data_tmp,
										 &data](eprosima::fastdds::dds::DataWriter* writer) {
		//data_.time_service(TimeConverter::GetTime_LLmcs());
		data = dds_data_tmp.back();
		dds_data_tmp.pop_back();
		if (!writer->write(&data))
		{
			return false;
		}
		return true;
	};

	BeforeTopicSendData before_topic_send_data = [&dds_data_tmp](void* data) {
		//data_.time_service(TimeConverter::GetTime_LLmcs());
		auto tmp = dds_data_tmp.back();
		(*static_cast<DDSData*>(data)) = tmp;
		dds_data_tmp.pop_back();
	};

	mypub->initPublishers();

	ServiceConfigForTest<subscriber::Configure> sub_config({"Participant_sub",
															Transport::TCP,
															"127.0.0.1",
															4043,
															{"127.0.0.1"},
															{},
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000,
															10000});
	AdditionalTopicInfo info = getAdditionalTopicInfo(100);
	subscriber::Configure sub_ddsdata_config = {
		0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, 100, 100, info, true};
	sub_config.configs = {sub_ddsdata_config};

	auto cacher = std::make_shared<DataCacher>(100, info);
	subscriber::Service* mysub = new subscriber::Service(sub_config, cacher);

	//******************** WS ********************//
	ThreadSafeDeque<oatpp::String> client_cache;
	ws::OnMessageRead on_message_read = [&client_cache](const oatpp::String& message) {
		client_cache.push_back(message);
	};

	bool ws_server_stop = false;
	ws::TestCallback test_callback = [&ws_server_stop](ws::Group& group) {
		while (!group.hasClients())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		while (!ws_server_stop)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			group.sendCacherDataToAllAsync();
		}
		group.sendCloseToAllAsync();
	};

	std::thread ws_thread([&test_callback, &on_message_read, cacher]() {
		runWsConnection(test_callback, on_message_read, cacher);
	});
	//******************** WS ********************//

	std::thread pub_thread([mypub, mysub, &before_topic_send]() {
		mypub->testRunPublishers(before_topic_send);
		mysub->stopSubscribers();
	});

	mysub->initSubscribers();
	mysub->runSubscribers();

	pub_thread.join();
	delete mypub;
	delete mysub;

	ws_server_stop = true;
	ws_thread.join();

	DdsDataMapper dds_data_mapper;
	MediateDtoMapper mediate_dto_mapper;

	EXPECT_EQ(client_cache.size(), dds_datas.size());
	if (dds_datas.size() < client_cache.size())
		return;
	for (int i = 0; i < 100; ++i)
	{
		auto ws_data = std::string(client_cache.pop_front().value()->c_str());
		auto dds_data = mediate_dto_mapper.toString(
			dds_data_mapper.toMediateDataDto(dds_datas[100 - i - 1], info));
		EXPECT_EQ(ws_data, dds_data);
	}
}

//TEST(DdsWsService, OneToOneConnection)
//{
//	OneToOneConnection();
//}

} // namespace scada_ate

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

#ifndef _DEBUG
	system("pause");
#endif

	return 0;
}
