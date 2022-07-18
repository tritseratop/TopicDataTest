#include "Utilities/Ws/TestUtility.h"

#include "Utilities/Common/TimeConverter/TimeConverter.hpp"

namespace scada_ate::ws
{
void to_json(nlohmann::json& json, const TestPacket& packet)
{
	json = {{"disp", packet.disp}, {"str", packet.str}};
}

void from_json(const nlohmann::json& json, TestPacket& packet)
{
	json.at("disp").get_to(packet.disp);
	json.at("str").get_to(packet.str);
}

TestPacket createTestPacket(size_t str_size)
{
	return {1'000'000'000'000'000, std::string(str_size, 'a')};
}

void insertTimeToJson(oatpp::String str)
{
	std::string time = std::to_string(TimeConverter::GetTime_LLmcs());
	auto first = str->find_first_of(':') + 1;
	auto last = str->find_first_of(',');
	str->erase(first, last - first);
	str->insert(str->find_first_of(':') + 1, time);
}

void replaceTimeToJson(oatpp::String str)
{
	std::string time = std::to_string(TimeConverter::GetTime_LLmcs());
	auto first = str->find_first_of(':') + 1;
	auto last = str->find_first_of(',');
	str->replace(first, last - first, time);
}

void replaceTimeToJson(oatpp::String str, int64_t time)
{
	std::string time_str = std::to_string(time);
	auto first = str->find_first_of(':') + 1;
	auto last = str->find_first_of(',');
	str->replace(first, last - first, time_str);
}

int64_t getTimeFromJsonString(oatpp::String str)
{
	auto first = str->find_first_of(':') + 1;
	auto last = str->find_first_of(',');
	std::string time_s = str->substr(first, last - first);
	int64_t time = std::stoll(time_s);

	return time;
}
} // namespace scada_ate::ws