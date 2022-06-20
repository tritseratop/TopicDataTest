#include "WsTestUtility.h"

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
	auto vectors = VectorsForData(size, char_size);
	auto tag_info = getAdditionalTopicInfo(size);

	WsDataUnion data_union;
	data_union.data_dto = getMediateDataDto(vectors, tag_info);

	auto collect_int = WsDataCollectionInt::createShared();
	auto collect_float = WsDataCollectionFloat::createShared();
	auto collect_double = WsDataCollectionDouble::createShared();
	auto collect_char = WsDataCollectionChar::createShared();

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

	collect_int->qlt = oatpp::String(vectors.int_qualities.data(), vectors.int_qualities.size());
	collect_float->qlt = oatpp::String(vectors.float_qualities.data(),
									   vectors.float_qualities.size());
	collect_double->qlt = oatpp::String(vectors.double_qualities.data(),
										vectors.double_qualities.size());
	collect_char->qlt = oatpp::String(vectors.char_qualities.data(), vectors.char_qualities.size());

	for (int i = 0; i < size; ++i)
	{
		collect_int->tsrc->push_back(oatpp::Int64(vectors.time_values[i]));
		collect_int->tag->push_back(
			oatpp::UInt32(tag_info.tags.at(DataCollectiionType::DATA_INT)[i]));
		collect_int->val->push_back(oatpp::Int32(vectors.int_values[i]));

		collect_float->tsrc->push_back(oatpp::Int64(vectors.time_values[i]));
		collect_float->tag->push_back(
			oatpp::UInt32(tag_info.tags.at(DataCollectiionType::DATA_FLOAT)[i]));
		collect_float->val->push_back(oatpp::Float32(vectors.float_values[i]));

		collect_double->tsrc->push_back(oatpp::Int64(vectors.time_values[i]));
		collect_double->tag->push_back(
			oatpp::UInt32(tag_info.tags.at(DataCollectiionType::DATA_DOUBLE)[i]));
		collect_double->val->push_back(oatpp::Float64(vectors.double_values[i]));

		collect_char->tsrc->push_back(oatpp::Int64(vectors.time_values[i]));
		collect_char->tag->push_back(
			oatpp::UInt32(tag_info.tags.at(DataCollectiionType::DATA_CHAR)[i]));

		oatpp::String str(vectors.char_value.size());
		std::copy(vectors.char_value.begin(), vectors.char_value.end(), str->begin());
		collect_char->val->push_back(str);
	}

	data_union.ws_dto = WsDataDto::createShared();

	data_union.ws_dto->tsrv = vectors.time_service;
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
