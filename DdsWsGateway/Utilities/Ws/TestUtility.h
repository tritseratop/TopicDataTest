#ifndef WS_TEST_UTILITY_H_
#define WS_TEST_UTILITY_H_

#include "Utilities/Common/Generators.h"
#include "Utilities/Common/nlohmann/json.hpp"

#include "oatpp/core/Types.hpp"

#include <functional>

namespace scada_ate::ws
{
struct TestPacket
{
	int64_t disp = 0;
	std::string str = "";

	friend void to_json(nlohmann::json& json, const TestPacket& packet);

	friend void from_json(const nlohmann::json& json, TestPacket& packet);
};

TestPacket createTestPacket(size_t str_size);

void insertTimeToJson(oatpp::String str);

void replaceTimeToJson(oatpp::String str);
void replaceTimeToJson(oatpp::String str, int64_t time);

int64_t getTimeFromJsonString(oatpp::String str);
} // namespace scada_ate::ws

#endif //!WS_TEST_UTILITY_H_