#ifndef WS_TEST_UTILITY_H_
#define WS_TEST_UTILITY_H_

#include <functional>

#include "DdsTestUtility.h"
#include "Lib/WsService/Server.h"
#include "Utilities/TestUtilities/CommonTestUtilities.h"
#include "Utilities/WsDto.h"
#include "Utilities/nlohmann/json.hpp"

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

#endif //!WS_TEST_UTILITY_H_