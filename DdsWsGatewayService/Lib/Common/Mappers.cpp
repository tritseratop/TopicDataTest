#include "Lib/Common/Mappers.h"

namespace scada_ate
{
using namespace dds;

MediateDataDto DdsDataMapper::toMediateDataDto(DDSData dataset, const MappingInfo& info)
{
	auto tags = info.tags;
	for (auto& [type, tag] : tags)
	{
		tag.resize(dataset.data_char().value().size());
	}

	std::vector<std::vector<char>> data_char;
	data_char.reserve(dataset.data_char().value().size());

	for (int i = 0; i < dataset.data_char().value().size(); ++i)
	{
		data_char.push_back(std::move(dataset.data_char().value()[i].value()));
	}

	MediateDataDto result{dataset.time_service(),

						  {std::vector(dataset.data_int().value().size(), dataset.time_source()),
						   std::move(tags.at(DatasetType::DATA_INT)),
						   std::move(dataset.data_int().value()),
						   std::move(dataset.data_int().quality())},

						  {std::vector(dataset.data_float().value().size(), dataset.time_source()),
						   std::move(tags.at(DatasetType::DATA_FLOAT)),
						   std::move(dataset.data_float().value()),
						   std::move(dataset.data_float().quality())},

						  {std::vector(dataset.data_double().value().size(), dataset.time_source()),
						   std::move(tags.at(DatasetType::DATA_DOUBLE)),
						   std::move(dataset.data_double().value()),
						   std::move(dataset.data_double().quality())},

						  {std::vector(dataset.data_char().value().size(), dataset.time_source()),
						   std::move(tags.at(DatasetType::DATA_CHAR)),
						   std::move(data_char),
						   std::move(dataset.data_char().quality())}};
	return result;
}

MediateDataDto DdsDataMapper::toMediateDataDtoOnlyDifferent(const DDSData& current_dataset,
															const MediateDataDto& prev_dto,
															const MappingInfo& info)
{
	return MediateDataDto();
}

MediateDataDto DdsDataExMapper::toMediateDataDto(const DDSDataEx& current_dataset,
												 const MappingInfo& info,
												 const MediateDataDto& prev_dto)
{
	auto result = prev_dto;
	result.time_service = current_dataset.time_service();
	insertDdsDataEx(
		result.data_int, current_dataset.data_int(), info.tag_to_index.at(DatasetType::DATA_INT));
	insertDdsDataEx(result.data_float,
					current_dataset.data_float(),
					info.tag_to_index.at(DatasetType::DATA_FLOAT));
	insertDdsDataEx(result.data_double,
					current_dataset.data_double(),
					info.tag_to_index.at(DatasetType::DATA_DOUBLE));
	insertDdsDataEx(result.data_char,
					current_dataset.data_char(),
					info.tag_to_index.at(DatasetType::DATA_CHAR));

	return result;
}

template<class T, class DdsDataSequence>
void insertDdsDataEx(DataSequence<T>& sequence_to_change,
					 const std::vector<DdsDataSequence>& current_sequence,
					 const TagToIndex& tag_to_index)
{
	for (auto sample : current_sequence)
	{
		uint32_t index = tag_to_index.at(sample.id_tag());
		if (index >= sequence_to_change.size())
		{
			sequence_to_change.resize(index + 1);
		}
		insertSample(sequence_to_change, sample, index);
	}
}

template<class T, class DdsDataSequence>
void insertSample(DataSequence<T>& sequence, const DdsDataSequence& sample, uint32_t index)
{
	sequence.time_source[index] = sample.time_source();
	sequence.id_tag[index] = sample.id_tag();
	sequence.value[index] = sample.value();
	sequence.quality[index] = sample.quality();
}

MediateDataDto DdsDataExMapper::toMediateDataDtoOnlyDifferent(const DDSDataEx& current_dataset,
															  const MappingInfo& info,
															  MediateDataDto prev_dto)
{
	prev_dto.time_service = current_dataset.time_service();
	selectOnlyDifferent(prev_dto.data_int,
						std::move(current_dataset.data_int()),
						info.tag_to_index.at(DatasetType::DATA_INT));
	selectOnlyDifferent(prev_dto.data_float,
						std::move(current_dataset.data_float()),
						info.tag_to_index.at(DatasetType::DATA_FLOAT));
	selectOnlyDifferent(prev_dto.data_double,
						std::move(current_dataset.data_double()),
						info.tag_to_index.at(DatasetType::DATA_DOUBLE));
	selectOnlyDifferent(prev_dto.data_char,
						std::move(current_dataset.data_char()),
						info.tag_to_index.at(DatasetType::DATA_CHAR));

	return prev_dto;
}

template<class T, class DdsDataSequence>
void selectOnlyDifferent(DataSequence<T> sequence_to_change,
						 const std::vector<DdsDataSequence>& current_sequence,
						 const TagToIndex& tag_to_index)
{
	DataSequence<T> result;
	for (const auto& sample : current_sequence)
	{
		uint32_t index = tag_to_index.at(sample.id_tag());
		if (!isEqualValues(sequence_to_change, sample, index))
		{
			pushBackSample(result, sample);
		}
	}
}

template<class T, class DdsDataSequence>
bool isEqualValues(const DataSequence<T>& sequence, const DdsDataSequence& sample, uint32_t index)
{
	return sequence.id_tag[index] == sample.id_tag() && sequence.value[index] == sample.value()
		   && sequence.quality[index] == sample.quality();
}

template<class T, class DdsDataSequence>
void pushBackSample(DataSequence<T>& sequence, const DdsDataSequence& sample)
{
	sequence.time_source.push_back(sample.time_source());
	sequence.id_tag.push_back(sample.id_tag());
	sequence.value.push_back(sample.value());
	sequence.quality.push_back(sample.quality());
}

//template<class T>
//void insertDdsDataEx(DataSequence<T>& sequence_to_change,
//					 std::vector<DataExChar> current_sequence,
//					 const TagToIndex& tag_to_index)
//{
//	for (auto sample : std::move(current_sequence))
//	{
//		insertSample(sequence_to_change, std::move(sample), tag_to_index);
//	}
//}

MediateAlarmDto DdsAlarmMapper::toMediateAlarmDto(DDSAlarm dataset, const MappingInfo& info)
{
	auto tags = info.tags.at(DatasetType::ALARM_UINT32);
	tags.resize(dataset.alarms().size());

	MediateAlarmDto result{dataset.time_service(),
						   std::vector(dataset.alarms().size(), dataset.time_source()),
						   std::move(tags),
						   std::move(dataset.alarms()),
						   std::move(dataset.quality())};
	return result;
}

MediateAlarmDto DdsAlarmExMapper::toMediateAlarmDto(MediateAlarmDto prev_dto,
													const DDSAlarmEx& current_dataset,
													const MappingInfo& info)
{
	prev_dto.time_service = current_dataset.time_service();
	/*insertDdsDataEx(
		prev_dto, current_dataset.alarms(), info.tag_to_index.at(DatasetType::ALARM_UINT32));*/
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