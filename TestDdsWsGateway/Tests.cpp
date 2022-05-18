#include "../DdsWsGatewayService/Lib/DdsService/DdsSubscriber.h"
#include "../DdsWsGatewayService/Lib/Notifier/Notifier.h"
#include "../DdsWsGatewayService/Lib/WsService/WsServer.h"
#include "../DdsWsGatewayService/Utilities/DdsTestUtility.h"
#include "../DdsWsGatewayService/Utilities/PackageAnalyser.h"
#include "../DdsWsGatewayService/Utilities/WsTestUtility.h"
#include "../DdsWsGatewayService/Utilities/nlohmann/json.hpp"

#include "../WsClient/WSClient.hpp"

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

TEST(DtoTest, DdsDataConversation)
{
	auto dds_data = getEqualDdsData(4);
	DdsDataMapper ddsdata_mapper;
	DdsDataExMapper ddsdataex_mapper;

	AdditionalTopicInfo info = getAdditionalTopicInfo(4);

	MediateDataDto d = ddsdata_mapper.toMediateDataDto(std::move(dds_data.first), info);
	EXPECT_EQ(dds_data.second, d);

	DataExUnion data_ex_union = getEqualDdsDataEx(3, 5);
	MediateDataDto d2 = ddsdataex_mapper.toMediateDataDto(
		data_ex_union.data_ex, info, data_ex_union.dto_to_change);
	EXPECT_EQ(d2, data_ex_union.dto);

	DataExUnion data_ex_union1 = getEqualDdsDataEx(3, 0);
	MediateDataDto d3 = ddsdataex_mapper.toMediateDataDto(
		data_ex_union1.data_ex, info, data_ex_union1.dto_to_change);
	EXPECT_EQ(d3, data_ex_union1.dto);

	info = getAdditionalTopicInfo(500);
	DataExUnion data_ex_union2 = getEqualDdsDataEx(500, 1000);
	MediateDataDto d4 = ddsdataex_mapper.toMediateDataDto(
		data_ex_union2.data_ex, info, data_ex_union2.dto_to_change);
	EXPECT_EQ(d4, data_ex_union2.dto);

	DataExUnion data_ex_union3 = getEqualDdsDataEx(0, 0);
	MediateDataDto d5 = ddsdataex_mapper.toMediateDataDto(
		data_ex_union3.data_ex, info, data_ex_union3.dto_to_change);
	EXPECT_EQ(d5, data_ex_union3.dto);

	DataExUnion data_ex_union4 = getEqualDdsDataEx(0, 3);
	MediateDataDto d6 = ddsdataex_mapper.toMediateDataDto(
		data_ex_union4.data_ex, info, data_ex_union4.dto_to_change);
	EXPECT_EQ(d6, data_ex_union4.dto);
}

TEST(DtoTest, WsDataConversation)
{
	MediateDtoMapper mapper;
	auto serializeConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
	auto deserializeConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();

	/* enable beautifier */
	serializeConfig->useBeautifier = true;
	auto json_object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared(
		serializeConfig, deserializeConfig);

	auto data = getWsDataUnion(20, 1);
	auto dto = mapper.toWsDataDto(data.data_dto);
	EXPECT_EQ(json_object_mapper->writeToString(data.ws_dto),
			  json_object_mapper->writeToString(dto));

	auto data1 = getWsDataUnion(0);
	auto dto1 = mapper.toWsDataDto(data1.data_dto);
	EXPECT_EQ(json_object_mapper->writeToString(data1.ws_dto),
			  json_object_mapper->writeToString(dto1));
}

TEST(DtoTest, MediateDtoToString)
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

void runServerSending(WebsockServer* server, int64_t initial_disp, int64_t sleep, size_t str_size)
{
	while (!server->isConnected())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	auto packet = TestPacket({initial_disp, std::string(str_size, 'a')});
	for (int i = 0; i < 100; ++i)
	{
		packet.disp = initial_disp + i;
		auto packet_json = nlohmann::json(packet).dump();
		auto oatpp_packet_json = oatpp::String(packet_json);
		server->sendData(oatpp_packet_json);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
	}
	server->sendClose();
	server->stop();
}

TEST(WsConnectionTest, DataTransmition)
{
	oatpp::base::Environment::init();
	{
		int64_t initisl_disp = 1'000'000'000'000'000;

		Configure ws_conf;
		ws_conf.WS_HOST = "127.0.0.1";
		WebsockServer server(ws_conf);
		std::thread sending([&server]() { server.run(); });
		std::thread server_thread(
			[initisl_disp, &server]() { runServerSending(&server, initisl_disp, 0, 1); });

		Configure configure;
		configure.WS_HOST = "127.0.0.1";
		WSClient wsclient(configure);
		wsclient.run();

		server_thread.join();
		sending.join();

		auto server_cache = server.getCache();
		EXPECT_EQ(100, server_cache.size());
		for (int i = 0; i < 100; ++i)
		{
			auto server_disp = server_cache.front();

			EXPECT_EQ(initisl_disp + i, server_disp);
			server_cache.pop_front();
		}

		auto cache = wsclient.getCache();
		EXPECT_EQ(100, cache.size());

		if (cache.size() < 100)
			return;
		for (int i = 0; i < 100; ++i)
		{
			auto disp = cache.front();

			EXPECT_EQ(initisl_disp + i, disp);
			cache.pop_front();
		}
	}
	oatpp::base::Environment::destroy();
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	system("pause");
}
