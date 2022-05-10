#include "../DdsWsGatewayService/Lib/DataObserver/DataObserver.h"
#include "../DdsWsGatewayService/Lib/DdsSubscriber.h"
#include "../DdsWsGatewayService/Lib/WsService/WsServer.h"
#include "../DdsWsGatewayService/Utilities/DdsTestUtility.h"
#include "../DdsWsGatewayService/Utilities/WsTestUtility.h"
#include "../DdsWsGatewayService/Utilities/nlohmann/json.hpp"

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
	DdsTopicToMediateDtoMapper mapper;

	AdditionalTopicInfo info = getAdditionalTopicInfo(4);

	MediateDataDto d = mapper.mapDdsData(std::move(dds_data.first), info);
	EXPECT_EQ(dds_data.second, d);

	DataExUnion data_ex_union = getEqualDdsDataEx(3, 5);
	MediateDataDto d2 =
		mapper.mapDdsDataEx(data_ex_union.dto_to_change, data_ex_union.data_ex, info);
	EXPECT_EQ(d2, data_ex_union.dto);

	DataExUnion data_ex_union1 = getEqualDdsDataEx(3, 0);
	MediateDataDto d3 =
		mapper.mapDdsDataEx(data_ex_union1.dto_to_change, data_ex_union1.data_ex, info);
	EXPECT_EQ(d3, data_ex_union1.dto);

	info = getAdditionalTopicInfo(500);
	DataExUnion data_ex_union2 = getEqualDdsDataEx(500, 1000);
	MediateDataDto d4 =
		mapper.mapDdsDataEx(data_ex_union2.dto_to_change, data_ex_union2.data_ex, info);
	EXPECT_EQ(d4, data_ex_union2.dto);

	DataExUnion data_ex_union3 = getEqualDdsDataEx(0, 0);
	MediateDataDto d5 =
		mapper.mapDdsDataEx(data_ex_union3.dto_to_change, data_ex_union3.data_ex, info);
	EXPECT_EQ(d5, data_ex_union3.dto);

	DataExUnion data_ex_union4 = getEqualDdsDataEx(0, 3);
	MediateDataDto d6 =
		mapper.mapDdsDataEx(data_ex_union4.dto_to_change, data_ex_union4.data_ex, info);
	EXPECT_EQ(d6, data_ex_union4.dto);
}

TEST(DtoTest, WsDataConversation)
{
	MediateDtoToWsDtoMapper ws_mapper;
	std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> json_object_mapper =
		oatpp::parser::json::mapping::ObjectMapper::createShared();

	auto data = getWsDataUnion(20);
	auto dto = ws_mapper.mapDataDto(data.data_dto);
	EXPECT_EQ(json_object_mapper->writeToString(data.ws_dto),
			  json_object_mapper->writeToString(dto));

	auto data1 = getWsDataUnion(0);
	auto dto1 = ws_mapper.mapDataDto(data1.data_dto);
	EXPECT_EQ(json_object_mapper->writeToString(data1.ws_dto),
			  json_object_mapper->writeToString(dto1));
}

TEST(JsonParsing, JsonToTestConditionParsing)
{
	std::ifstream ifile("test.json");
	nlohmann::json json;
	ifile >> json;
	auto result = parseJsonToGlobalTestConditions(json);
	GlobalTestConditions expected{{
									  {10, 10, 5, 100},
									  {100, 10, 5, 100},
									  {500, 10, 5, 100},
									  {1000, 10, 5, 100},
								  },
								  "127.0.0.1",
								  false,
								  false};

	EXPECT_EQ(result, expected);
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
		std::cout << "vectors size: " << c.all_vectors_sizes << "\t | "
				  << DDSData::getCdrSerializedSize(dds_data) << std::endl;
	}

	for (const auto& c : result.conditions)
	{
		auto dds_data_ex = getDdsDataEx(c.all_vectors_sizes, c.char_vector_sizes);
		std::cout << "vectors size: " << c.all_vectors_sizes << "\t | "
				  << DDSDataEx::getCdrSerializedSize(dds_data_ex) << std::endl;
	}
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
}