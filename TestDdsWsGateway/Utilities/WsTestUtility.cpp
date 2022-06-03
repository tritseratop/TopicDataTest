#include "WsTestUtility.h"

#include "../WsClient/WSClient.hpp"
#include "Lib/WsService/Server.h"
#include "Utilities/TimeConverter/TimeConverter.hpp"

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

WsDataUnion getWsDataUnion(size_t size, size_t char_size)
{
	WsDataUnion data_union;
	auto buf = getEqualDdsData(size, char_size);
	data_union.dds_data = buf.first;
	data_union.data_dto = buf.second;

	auto collect_int = WsDataCollectionInt::createShared();
	auto collect_float = WsDataCollectionFloat::createShared();
	auto collect_double = WsDataCollectionDouble::createShared();
	auto collect_char = WsDataCollectionChar::createShared();

	AdditionalTopicInfo info = getAdditionalTopicInfo(size);
	std::string quality(size, 'a');

	collect_int->qlt = quality.c_str();
	collect_float->qlt = quality.c_str();
	collect_double->qlt = quality.c_str();
	collect_char->qlt = quality.c_str();

	collect_int->tsrc = {};
	collect_int->tag = {};
	collect_int->val = {};

	collect_float->tsrc = {};
	collect_float->tag = {};
	collect_float->val = {};

	collect_double->tsrc = {};
	collect_double->tag = {};
	collect_double->val = {};

	collect_char->tsrc = {};
	collect_char->tag = {};
	collect_char->val = {};
	std::string char_val(char_size, 'a');

	for (int i = 0; i < size; ++i)
	{
		collect_int->tsrc->push_back(oatpp::Int64(101));
		collect_int->tag->push_back(oatpp::UInt32(info.tags.at(DataCollectiionType::DATA_INT)[i]));
		collect_int->val->push_back(oatpp::Int32(1));

		collect_float->tsrc->push_back(oatpp::Int64(101));
		collect_float->tag->push_back(
			oatpp::UInt32(info.tags.at(DataCollectiionType::DATA_FLOAT)[i]));
		collect_float->val->push_back(oatpp::Float32(1));

		collect_double->tsrc->push_back(oatpp::Int64(101));
		collect_double->tag->push_back(
			oatpp::UInt32(info.tags.at(DataCollectiionType::DATA_DOUBLE)[i]));
		collect_double->val->push_back(oatpp::Float64(1));

		collect_char->tsrc->push_back(oatpp::Int64(101));
		collect_char->tag->push_back(
			oatpp::UInt32(info.tags.at(DataCollectiionType::DATA_CHAR)[i]));
		collect_char->val->push_back(oatpp::String(char_val.c_str()));
	}

	data_union.ws_dto = WsDataDto::createShared();

	data_union.ws_dto->tsrv = 100;
	data_union.ws_dto->di = collect_int;
	data_union.ws_dto->df = collect_float;
	data_union.ws_dto->dd = collect_double;
	data_union.ws_dto->dc = collect_char;

	return data_union;
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

void runWsConnection(TestCallback& test_callback, OnMessageRead& on_message_read)
{
	oatpp::base::Environment::init();
	{
		int64_t init_disp = 1'000'000'000'000'000;

		const WsConfigure ws_conf;
		auto group = std::make_shared<Group>(0);
		std::unordered_map<int64_t, std::shared_ptr<Group>> groups;
		groups[group->getId()] = group;

		AppComponent component(ws_conf, groups);

		Server server;

		std::thread server_thread([&server]() { server.run(); });

		WSClient wsclient(ws_conf);
		std::thread client_thread(
			[&wsclient, &on_message_read]() { wsclient.run(on_message_read); });

		group->runTestMessageSending(std::move(test_callback));

		client_thread.join();
		server.stop();
		server_thread.join();
	}
	oatpp::base::Environment::destroy();
}
