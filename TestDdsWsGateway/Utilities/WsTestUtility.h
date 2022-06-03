#ifndef WS_TEST_UTILITY_H_
#define WS_TEST_UTILITY_H_

#include <functional>

#include "../../DdsWsGatewayService/Utilities/WsDto.h"
#include "../../DdsWsGatewayService/Utilities/nlohmann/json.hpp"
#include "../DdsWsGatewayService/Lib/WsService/Server.h"
#include "DdsTestUtility.h"

class Server;

struct WsDataUnion
{
	DDSData dds_data;
	MediateDataDto data_dto;
	WsDataDto::Wrapper ws_dto;
};

struct TestPacket
{
	int64_t disp = 0;
	std::string str = "";

	friend void to_json(nlohmann::json& json, const TestPacket& packet);

	friend void from_json(const nlohmann::json& json, TestPacket& packet);
};

TestPacket createTestPacket(size_t str_size);

WsDataUnion getWsDataUnion(size_t size = 4, size_t char_size = 1);

void insertTimeToJson(oatpp::String str);

void replaceTimeToJson(oatpp::String str);
void replaceTimeToJson(oatpp::String str, int64_t time);

int64_t getTimeFromJsonString(oatpp::String str);

void runWsConnection(TestCallback& test_callback, OnMessageRead& on_message_read);

#endif //!WS_TEST_UTILITY_H_