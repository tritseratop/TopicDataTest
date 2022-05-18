#include "Lib/Notifier/Mappers.h"

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

MediateDataDtoWithVectorsOfStruct
DdsDataMapper::toMediateDataDtoWithVectorsOfStruct(DDSData data, const AdditionalTopicInfo& info)
{
	size_t size = data.data_double().value().size();

	MediateDataDtoWithVectorsOfStruct result;
	result.data_int.reserve(size);
	result.data_float.reserve(size);
	result.data_double.reserve(size);
	result.data_char.reserve(size);

	for (int i = 0; i < size; ++i)
	{
		result.data_int.push_back({data.time_source(),
								   info.tags.at(DataCollectiionType::DATA_INT)[i],
								   data.data_int().value()[i],
								   data.data_int().quality()[i]});
		result.data_float.push_back({data.time_source(),
									 info.tags.at(DataCollectiionType::DATA_FLOAT)[i],
									 data.data_float().value()[i],
									 data.data_float().quality()[i]});
		result.data_double.push_back({data.time_source(),
									  info.tags.at(DataCollectiionType::DATA_DOUBLE)[i],
									  data.data_double().value()[i],
									  data.data_double().quality()[i]});
		result.data_char.push_back({data.time_source(),
									info.tags.at(DataCollectiionType::DATA_CHAR)[i],
									std::move(data.data_char().value()[i].value()),
									data.data_char().quality()[i]});
	}
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
	size_t n = cur_samples.size();
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

MediateDataDtoWithVectorsOfStruct
DdsDataExMapper::toMediateDataDtoWithVectorsOfStruct(DDSDataEx cur_data_ex,
													 MediateDataDtoWithVectorsOfStruct prev_dto,
													 const AdditionalTopicInfo& info)
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
void fillChanged(std::vector<DataSample<T>>& prev_dto_collection,
				 std::vector<DdsSample> cur_samples,
				 const TagToIndex& tag_to_index)
{
	size_t n = cur_samples.size();
	for (auto sample : std::move(cur_samples))
	{
		if (tag_to_index.at(sample.id_tag()) >= prev_dto_collection.size())
		{
			prev_dto_collection.resize(tag_to_index.at(sample.id_tag()) + 1);
		}
		prev_dto_collection[tag_to_index.at(sample.id_tag())].time_source = sample.time_source();
		prev_dto_collection[tag_to_index.at(sample.id_tag())].id_tag = sample.id_tag();
		prev_dto_collection[tag_to_index.at(sample.id_tag())].value = sample.value();
		prev_dto_collection[tag_to_index.at(sample.id_tag())].quality = sample.quality();
	}
}

template<class DdsSample>
void fillChanged(std::vector<DataSample<std::vector<char>>>& prev_dto_collection,
				 std::vector<DdsSample> cur_samples,
				 const TagToIndex& tag_to_index)
{
	size_t n = cur_samples.size();
	for (auto sample : std::move(cur_samples))
	{
		if (tag_to_index.at(sample.id_tag()) >= prev_dto_collection.size())
		{
			prev_dto_collection.resize(tag_to_index.at(sample.id_tag()) + 1);
		}
		prev_dto_collection[tag_to_index.at(sample.id_tag())].time_source = sample.time_source();
		prev_dto_collection[tag_to_index.at(sample.id_tag())].id_tag = sample.id_tag();
		prev_dto_collection[tag_to_index.at(sample.id_tag())].value = std::move(sample.value());
		prev_dto_collection[tag_to_index.at(sample.id_tag())].quality = sample.quality();
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

WsDataDto::Wrapper MediateDtoMapper::toWsDataDto(const MediateDataDto& data)
{
	auto collect_int = WsDataCollectionInt::createShared();
	fillVector(collect_int->tsrc, data.data_int.time_source);
	fillVector(collect_int->tag, data.data_int.id_tag);
	fillVector(collect_int->val, data.data_int.value);
	fillVector(collect_int->qlt, data.data_int.quality);

	auto collect_float = WsDataCollectionFloat::createShared();
	fillVector(collect_float->tsrc, data.data_float.time_source);
	fillVector(collect_float->tag, data.data_float.id_tag);
	fillVector(collect_float->val, data.data_float.value);
	fillVector(collect_float->qlt, data.data_float.quality);

	auto collect_double = WsDataCollectionDouble::createShared();
	fillVector(collect_double->tsrc, data.data_double.time_source);
	fillVector(collect_double->tag, data.data_double.id_tag);
	fillVector(collect_double->val, data.data_double.value);
	fillVector(collect_double->qlt, data.data_double.quality);

	auto collect_char = WsDataCollectionChar::createShared();
	fillVector(collect_char->tsrc, data.data_char.time_source);
	fillVector(collect_char->tag, data.data_char.id_tag);
	fillVector(collect_char->val, data.data_char.value);
	fillVector(collect_char->qlt, data.data_char.quality);

	auto ws_data_dto = WsDataDto::createShared();
	ws_data_dto->tsrv = data.time_service;
	ws_data_dto->di = collect_int;
	ws_data_dto->df = collect_float;
	ws_data_dto->dd = collect_double;
	ws_data_dto->dc = collect_char;

	return ws_data_dto;
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
