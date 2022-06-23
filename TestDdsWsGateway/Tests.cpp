#include "../DdsWsGatewayService/Lib/Common/Notifier.h"
#include "../DdsWsGatewayService/Lib/DdsService/DdsSubscriber.h"
#include "../DdsWsGatewayService/Utilities/PackageAnalyser.h"
#include "../DdsWsGatewayService/Utilities/nlohmann/json.hpp"

#include "../DdsWsGatewayService/Utilities/TestUtilities/DdsTestUtility.h"
#include "../DdsWsGatewayService/Utilities/TestUtilities/WsTestUtility.h"

#include "../PublisherProject/DdsPublisher.h"

#include "Helpers/Utilities.h"

#include "oatpp/parser/json/Beautifier.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include <gtest/gtest.h>

std::vector<SubscriberConfig> configs({
	{0, 100, "DDSData1", "DDSData", TopicType::DDS_DATA, 100, 100},
	//        {1, 10000, "DDSData2", "DDSData", TopicType::DDS_DATA, 100, 100},
	//        {2, 10000, "DDSData3", "DDSData", TopicType::DDS_DATA, 100, 100},
});

ServiceConfigForTest<SubscriberConfig> config({"Participant_sub",
											   Transport::TCP,
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

TEST(DtoToWsMapping, MediateDtoToWsWrapper)
{
	MediateDtoMapper mapper;
	auto serializeConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
	auto deserializeConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();

	/* enable beautifier */
	serializeConfig->useBeautifier = true;
	auto json_object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared(
		serializeConfig, deserializeConfig);

	{
		auto data = getWsDataUnion(100, 20);
		auto dto = mapper.toWsDataDto(data.data_dto);
		auto res1 = json_object_mapper->writeToString(data.ws_dto);
		auto res2 = json_object_mapper->writeToString(dto);
		EXPECT_EQ(res1, res2);
	}

	auto data = getWsDataUnion(20, 0);
	auto dto = mapper.toWsDataDto(data.data_dto);
	auto res1 = json_object_mapper->writeToString(data.ws_dto);
	auto res2 = json_object_mapper->writeToString(dto);
	EXPECT_EQ(res1, res2);

	auto data1 = getWsDataUnion(0, 20);
	auto dto1 = mapper.toWsDataDto(data1.data_dto);
	EXPECT_EQ(json_object_mapper->writeToString(data1.ws_dto),
			  json_object_mapper->writeToString(dto1));
}

TEST(DtoToWsMapping, MediateDtoToString)
{
	MediateDtoMapper mapper;
	auto data1 = getMediateDataDto(1, 1);
	EXPECT_NE(mapper.toString(data1), mapper.toStringWithCharVectors(data1));
}

TEST(JsonParsing, JsonToTestConditionParsing)
{
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

TEST(JsonParsing, GettingTime)
{
	WsDataUnion ws_data = getWsDataUnion(10, 10);
	ws_data.ws_dto->tsrv = 1'000'000'000'000'000;

	auto json_object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
	oatpp::String data = json_object_mapper->writeToString(ws_data.ws_dto);
	int64_t time = 2'222'222'222'222'222;
	replaceTimeToJson(data, time);

	EXPECT_EQ(time, getTimeFromJsonString(data));

	ws_data.ws_dto->tsrv = TimeConverter::GetTime_LLmcs();
	data = json_object_mapper->writeToString(ws_data.ws_dto);
	time = TimeConverter::GetTime_LLmcs();
	replaceTimeToJson(data, time);

	EXPECT_EQ(time, getTimeFromJsonString(data));

	ws_data = getWsDataUnion(5, 1);
	ws_data.ws_dto->tsrv = TimeConverter::GetTime_LLmcs();
	data = json_object_mapper->writeToString(ws_data.ws_dto);
	time = TimeConverter::GetTime_LLmcs();
	replaceTimeToJson(data, time);

	EXPECT_EQ(time, getTimeFromJsonString(data));

	ws_data = getWsDataUnion(10, 1);
	ws_data.ws_dto->tsrv = TimeConverter::GetTime_LLmcs();
	data = json_object_mapper->writeToString(ws_data.ws_dto);
	time = TimeConverter::GetTime_LLmcs();
	replaceTimeToJson(data, time);

	EXPECT_EQ(time, getTimeFromJsonString(data));
}

TEST(setTimeToJsonTest, replace)
{
	auto json_object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
	WsDataUnion ws_data = getWsDataUnion(2, 1);
	oatpp::String result = json_object_mapper->writeToString(ws_data.ws_dto);
	replaceTimeToJson(result);
	auto dto = json_object_mapper->readFromString<WsDataDto::Wrapper>(result);
	auto expected = json_object_mapper->writeToString(dto);
	EXPECT_EQ(result.getValue(""), expected.getValue(""));
}

TEST(setTimeToJsonTest, insert)
{
	auto json_object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
	WsDataUnion ws_data = getWsDataUnion(2, 1);
	oatpp::String result = json_object_mapper->writeToString(ws_data.ws_dto);
	insertTimeToJson(result);
	auto dto = json_object_mapper->readFromString<WsDataDto::Wrapper>(result);
	auto expected = json_object_mapper->writeToString(dto);
	EXPECT_EQ(result.getValue(""), expected.getValue(""));
}

TEST(WsConnectionTest, RunWithoutCoroutine)
{
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
	TestCallback test_callback = [&test_packets, &before_msg_send](Group& adapter) {
		while (!adapter.hasClients())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		while (!test_packets.empty())
		{
			auto message = nlohmann::json(test_packets.back()).dump();
			test_packets.pop_back();
			adapter.sendTextMessageToAllAsync(oatpp::String(message), before_msg_send);
		}
		adapter.sendCloseToAllAsync();
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
	ServiceConfigForTest<PublisherConfig> pub_config({"Participant_pub",
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
	PublisherConfig ddsdata_config = {
		0, 10, 10, "DDSData", "DDSData", TopicType::DDS_DATA, 100, 2, false};
	pub_config.configs = {ddsdata_config};

	PublisherService* mypub = new PublisherService(pub_config);

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

	ServiceConfigForTest<SubscriberConfig> sub_config({"Participant_sub",
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
	SubscriberConfig sub_ddsdata_config = {
		0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, 100, 100, info, true};
	sub_config.configs = {sub_ddsdata_config};

	std::vector<DDSData> sub_datas;
	OnTopicReceived on_topic_received = [&sub_datas](std::shared_ptr<void> data_sample) {
		auto data_sample_ = std::static_pointer_cast<DDSData>(data_sample);
		sub_datas.push_back(*data_sample_);
	};

	auto cacher = std::make_shared<DataCacher>(5, info);
	SubscriberService* mysub = new SubscriberService(sub_config, cacher, on_topic_received);

	std::thread pub_thread([mypub, mysub, &before_topic_send]() {
		mypub->testRunPublishers(before_topic_send);
		mysub->stopSubscribers();
	});

	mysub->initSubscribers();
	mysub->runSubscribers();

	pub_thread.join();
	delete mypub;
	delete mysub;

	EXPECT_EQ(dds_datas.size(), sub_datas.size());
	if (dds_datas.size() != sub_datas.size())
		return;

	for (size_t i = 0; i < 100; ++i)
	{
		EXPECT_EQ(dds_datas[100 - i - 1], sub_datas[i]);
	}
}

TEST(DdsConnectionTest, DdsDataTest)
{
	DdsDataTest();
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
}
