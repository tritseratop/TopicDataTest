#include "Lib/Common/Mappers.h"

namespace scada_ate
{
using namespace dds;

MediateDataDto DdsDataMapper::toMediateDataDto(DDSData data, const AdditionalTopicInfo& info)
{
	std::vector<std::vector<char>> data_char;
	data_char.reserve(data.data_char().value().size());

	auto tags = info.tags;
	for (auto& [type, tag] : tags)
	{
		tag.resize(data.data_char().value().size());
	}

	for (int i = 0; i < data.data_char().value().size(); ++i)
	{
		data_char.push_back(std::move(data.data_char().value()[i].value()));
	}

	MediateDataDto result{data.time_service(),

						  {std::vector(data.data_int().value().size(), data.time_source()),
						   std::move(tags.at(DataCollectiionType::DATA_INT)),
						   std::move(data.data_int().value()),
						   std::move(data.data_int().quality())},

						  {std::vector(data.data_float().value().size(), data.time_source()),
						   std::move(tags.at(DataCollectiionType::DATA_FLOAT)),
						   std::move(data.data_float().value()),
						   std::move(data.data_float().quality())},

						  {std::vector(data.data_double().value().size(), data.time_source()),
						   std::move(tags.at(DataCollectiionType::DATA_DOUBLE)),
						   std::move(data.data_double().value()),
						   std::move(data.data_double().quality())},

						  {std::vector(data.data_char().value().size(), data.time_source()),
						   std::move(tags.at(DataCollectiionType::DATA_CHAR)),
						   std::move(data_char),
						   std::move(data.data_char().quality())}};
	return result;
}

MediateDataDto DdsDataExMapper::toMediateDataDto(DDSDataEx cur_data_ex,
												 const AdditionalTopicInfo& info,
												 MediateDataDto prev_dto)
{
	prev_dto.time_service = cur_data_ex.time_service();
	fillChanged(prev_dto.data_int,
				std::move(cur_data_ex.data_int()),
				info.tag_to_index.at(DataCollectiionType::DATA_INT));
	fillChanged(prev_dto.data_float,
				std::move(cur_data_ex.data_float()),
				info.tag_to_index.at(DataCollectiionType::DATA_FLOAT));
	fillChanged(prev_dto.data_double,
				std::move(cur_data_ex.data_double()),
				info.tag_to_index.at(DataCollectiionType::DATA_DOUBLE));
	fillChanged(prev_dto.data_char,
				std::move(cur_data_ex.data_char()),
				info.tag_to_index.at(DataCollectiionType::DATA_CHAR));

	return prev_dto;
}

template<class T, class DdsSample>
void fillChanged(DataSampleSequence<T>& prev_dto_collection,
				 std::vector<DdsSample> cur_samples,
				 const TagToIndex& tag_to_index)
{
	for (auto sample : std::move(cur_samples))
	{
		if (tag_to_index.at(sample.id_tag()) >= prev_dto_collection.size())
		{
			prev_dto_collection.resize(tag_to_index.at(sample.id_tag()) + 1);
		}
		prev_dto_collection.time_source[tag_to_index.at(sample.id_tag())] = sample.time_source();
		prev_dto_collection.id_tag[tag_to_index.at(sample.id_tag())] = sample.id_tag();
		prev_dto_collection.value[tag_to_index.at(sample.id_tag())] = sample.value();
		prev_dto_collection.quality[tag_to_index.at(sample.id_tag())] = sample.quality();
	}
}

template<class T>
void fillChanged(DataSampleSequence<T>& prev_dto_collection,
				 std::vector<DataExChar> cur_samples,
				 const TagToIndex& tag_to_index)
{
	size_t n = cur_samples.size();
	for (auto sample : std::move(cur_samples))
	{
		if (tag_to_index.at(sample.id_tag()) >= prev_dto_collection.size())
		{
			prev_dto_collection.resize(tag_to_index.at(sample.id_tag()) + 1);
		}
		prev_dto_collection.time_source[tag_to_index.at(sample.id_tag())] = sample.time_source();
		prev_dto_collection.id_tag[tag_to_index.at(sample.id_tag())] = sample.id_tag();
		prev_dto_collection.value[tag_to_index.at(sample.id_tag())] = std::move(sample.value());
		prev_dto_collection.quality[tag_to_index.at(sample.id_tag())] = sample.quality();
	}
}

MediateAlarmDto DdsAlarmMapper::toMediateAlarmDto(DDSAlarm data, const AdditionalTopicInfo& info)
{
	auto tags = info.tags.at(DataCollectiionType::ALARM_UINT32);
	tags.resize(data.alarms().size());

	MediateAlarmDto result{data.time_service(),
						   std::vector(data.alarms().size(), data.time_source()),
						   std::move(tags),
						   std::move(data.alarms()),
						   std::move(data.quality())};
	return result;
}

MediateAlarmDto DdsAlarmExMapper::toMediateAlarmDto(MediateAlarmDto prev_dto,
													const DDSAlarmEx& cur_data_ex,
													const AdditionalTopicInfo& info)
{
	prev_dto.time_service = cur_data_ex.time_service();
	/*fillChanged(
		prev_dto, cur_data_ex.alarms(), info.tag_to_index.at(DataCollectiionType::ALARM_UINT32));*/
	return prev_dto;
}

std::string MediateDtoMapper::toString(const MediateDataDto& dto)
{
	nlohmann::json json;
	json["tsrv"] = dto.time_service;

	json["di"]["tsrc"] = dto.data_int.time_source;
	json["di"]["tag"] = dto.data_int.id_tag;
	json["di"]["val"] = dto.data_int.value;
	json["di"]["qlt"] = convertCharVectorToString(dto.data_int.quality);

	json["df"]["tsrc"] = dto.data_float.time_source;
	json["df"]["tag"] = dto.data_float.id_tag;
	json["df"]["val"] = dto.data_float.value;
	json["df"]["qlt"] = convertCharVectorToString(dto.data_float.quality);

	json["dd"]["tsrc"] = dto.data_double.time_source;
	json["dd"]["tag"] = dto.data_double.id_tag;
	json["dd"]["val"] = dto.data_double.value;
	json["dd"]["qlt"] = convertCharVectorToString(dto.data_double.quality);

	json["dc"]["tsrc"] = dto.data_char.time_source;
	json["dc"]["tag"] = dto.data_char.id_tag;
	for (auto c : dto.data_char.value)
	{
		json["dc"]["val"].push_back(convertCharVectorToString(c));
	}
	json["dc"]["qlt"] = convertCharVectorToString(dto.data_char.quality);

	return json.dump();
}

std::string convertCharVectorToString(const std::vector<char>& chars)
{
	return std::string(chars.begin(), chars.end());
}

std::string MediateDtoMapper::toStringWithCharVectors(const MediateDataDto& dto)
{
	nlohmann::json json;
	json["tsrv"] = dto.time_service;

	json["di"]["tsrc"] = dto.data_int.time_source;
	json["di"]["tag"] = dto.data_int.id_tag;
	json["di"]["val"] = dto.data_int.value;
	pushBackContainerWithChars(std::back_inserter(json["di"]["qlt"]), dto.data_int.quality);

	json["df"]["tsrc"] = dto.data_float.time_source;
	json["df"]["tag"] = dto.data_float.id_tag;
	json["df"]["val"] = dto.data_float.value;
	pushBackContainerWithChars(std::back_inserter(json["df"]["qlt"]), dto.data_int.quality);

	json["dd"]["tsrc"] = dto.data_double.time_source;
	json["dd"]["tag"] = dto.data_double.id_tag;
	json["dd"]["val"] = dto.data_double.value;
	pushBackContainerWithChars(std::back_inserter(json["dd"]["qlt"]), dto.data_int.quality);

	json["dc"]["tsrc"] = dto.data_char.time_source;
	json["dc"]["tag"] = dto.data_char.id_tag;
	for (const auto& c : dto.data_char.value)
	{
		json["dc"]["val"].push_back(convertCharVectorToString(c));
	}
	pushBackContainerWithChars(std::back_inserter(json["dc"]["qlt"]), dto.data_int.quality);

	return json.dump();
}

template<class T>
void pushBackContainerWithChars(std::back_insert_iterator<T> result_it,
								const std::vector<char>& chars)
{
	auto convert_char_to_string = [](char c) { return std::string(1, c); };
	std::transform(chars.begin(), chars.end(), result_it, convert_char_to_string);
}

template<class OatppT, class T>
void MediateDtoMapper::fillVector(oatpp::Vector<OatppT>& oatpp_v, const std::vector<T>& v)
{
	oatpp_v = {};
	oatpp_v->reserve(v.size());
	for (T elem : v)
	{
		oatpp_v->push_back(OatppT(elem));
	}
}

void MediateDtoMapper::fillVector(oatpp::Vector<oatpp::String>& oatpp_v,
								  const std::vector<std::vector<char>>& v)
{
	oatpp_v = {};
	oatpp_v->reserve(v.size());
	for (auto elem : v)
	{
		std::string str(elem.begin(), elem.end());
		oatpp_v->push_back(oatpp::String(str.c_str()));
	}
}

void MediateDtoMapper::fillVector(oatpp::String& oatpp_v, const std::vector<char>& v)
{
	std::string str(v.begin(), v.end());
	oatpp_v = oatpp::String(str.c_str());
}
} // namespace scada_ate