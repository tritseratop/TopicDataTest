#include "Utilities/Dds/TestUtility.h"

#include <random>

namespace scada_ate::dds
{
MappingInfo getMappingInfo(size_t size)
{
	MappingInfo info;
	info.tag_to_index[DatasetType::DATA_INT];
	info.tag_to_index[DatasetType::DATA_FLOAT];
	info.tag_to_index[DatasetType::DATA_DOUBLE];
	info.tag_to_index[DatasetType::DATA_CHAR];
	info.tags[DatasetType::DATA_INT];
	info.tags[DatasetType::DATA_FLOAT];
	info.tags[DatasetType::DATA_DOUBLE];
	info.tags[DatasetType::DATA_CHAR];

	auto tags = generateVector<uint32_t>(size);

	for (auto& [data_type, tag_to_index] : info.tag_to_index)
	{
		info.tags.at(data_type).reserve(size);
		for (uint32_t i = 0; i < size; i++)
		{
			tag_to_index[tags[i]] = i;
			info.tags.at(data_type).push_back(tags[i]);
		}
	}
	info.topic_name = "Name";
	return info;
}

DDSData getDdsData(size_t size, size_t char_size)
{
	auto vectors = VectorsForData(size, char_size);
	return getDdsData(vectors);
}

DDSData getDdsData(const VectorsForData& vectors)
{
	DDSData data;

	data.time_service(vectors.time_service);
	if (!vectors.time_values.empty())
	{
		data.time_source(vectors.time_values[0]);
	}

	data.data_int().value(vectors.int_values);
	data.data_int().quality(vectors.int_qualities);

	data.data_float().value(vectors.float_values);
	data.data_float().quality(vectors.float_qualities);

	data.data_double().value(vectors.double_values);
	data.data_double().quality(vectors.double_qualities);

	DataChar data_char;
	data_char.value(vectors.char_value);
	data.data_char().value(getFilledVector(vectors.char_qualities.size(), data_char));
	data.data_char().quality(vectors.char_qualities);

	return data;
}

DDSDataEx getDdsDataEx(size_t size, size_t char_size)
{
	auto vectors = VectorsForData(size, char_size);
	auto tag_info = getMappingInfo(size);
	return getDdsDataEx(vectors, tag_info);
}

DDSDataEx getDdsDataEx(const VectorsForData& vectors, const MappingInfo& tag_info)
{
	DDSDataEx data;
	DataExInt data_ex_int;
	DataExFloat data_ex_float;
	DataExDouble data_ex_double;
	DataExChar data_ex_char;

	data.time_service(vectors.time_service);
	for (int i = 0; i < vectors.int_values.size(); ++i)
	{
		data_ex_int.time_source(vectors.time_values[i]);
		data_ex_int.id_tag(tag_info.tags.at(DatasetType::DATA_INT)[i]);
		data_ex_int.value(vectors.int_values[i]);
		data_ex_int.quality(vectors.int_qualities[i]);

		data_ex_float.time_source(vectors.time_values[i]);
		data_ex_float.id_tag(tag_info.tags.at(DatasetType::DATA_FLOAT)[i]);
		data_ex_float.value(vectors.float_values[i]);
		data_ex_float.quality(vectors.float_qualities[i]);

		data_ex_double.time_source(vectors.time_values[i]);
		data_ex_double.id_tag(tag_info.tags.at(DatasetType::DATA_DOUBLE)[i]);
		data_ex_double.value(vectors.double_values[i]);
		data_ex_double.quality(vectors.double_qualities[i]);

		data_ex_char.time_source(vectors.time_values[i]);
		data_ex_char.id_tag(tag_info.tags.at(DatasetType::DATA_CHAR)[i]);
		data_ex_char.value(vectors.char_value);
		data_ex_char.quality(vectors.char_qualities[i]);

		data.data_int().push_back(data_ex_int);
		data.data_float().push_back(data_ex_float);
		data.data_double().push_back(data_ex_double);
		data.data_char().push_back(data_ex_char);
	}

	return data;
}

template<class T>
std::vector<T> getDdsSets(size_t vector_size, std::function<T()> getter)
{
	std::vector<T> result;
	result.reserve(vector_size);
	for (size_t i = 0; i < vector_size; ++i)
	{
		result.push_back(getter());
		result.back().time_service(i);
	}
	return result;
}

template<class T>
std::vector<std::vector<T>> getVectorOfDdsSets(size_t vector_size,
											   size_t sets_size,
											   std::function<T()> getter)
{
	std::vector<std::vector<T>> result(vector_size);
	int64_t digit = 0;
	for (auto& data_samples : result)
	{
		++digit;
		for (size_t i = 0; i < sets_size; ++i)
		{
			data_samples.push_back(getter());
			data_samples.back().time_service(digit * sets_size * 10 + i);
		}
	}
	return result;
}

DdsDataSets getDdsDataSets(size_t vector_size, size_t sequences_size, size_t char_size)
{
	std::vector<DDSData> result = getDdsSets<DDSData>(vector_size, [sequences_size, char_size]() {
		return getDdsData(sequences_size, char_size);
	});
	return result;
}

std::vector<DdsDataSets> getVectorOfDdsDataSets(size_t vector_size,
												size_t sets_size,
												size_t sequences_size,
												size_t char_size)
{
	std::vector<DdsDataSets> result = getVectorOfDdsSets<DDSData>(
		vector_size, sets_size, [sequences_size, char_size]() {
			return getDdsData(sequences_size, char_size);
		});
	return result;
}

DdsDataExSets getDdsDataExSets(size_t vector_size,
							   size_t sequences_size,
							   size_t char_size,
							   const MappingInfo& info)
{
	std::vector<DDSDataEx> result = getDdsSets<DDSDataEx>(
		vector_size, [sequences_size, char_size, &info]() {
			auto vectors = VectorsForData(sequences_size, char_size);
			return getDdsDataEx(vectors, info);
		});
	return result;
}

std::vector<DdsDataExSets> getVectorOfDdsDataExSets(size_t vector_size,
													size_t sets_size,
													size_t sequences_size,
													size_t char_size,
													const MappingInfo& info)
{
	std::vector<DdsDataExSets> result = getVectorOfDdsSets<DDSDataEx>(
		vector_size, sets_size, [sequences_size, char_size, &info]() {
			auto vectors = VectorsForData(sequences_size, char_size);
			return getDdsDataEx(vectors, info);
		});
	return result;
}

DataUnion getDataUnion(size_t size, size_t char_size)
{
	auto vectors = VectorsForData(size, char_size);
	auto tag_info = getMappingInfo(size);

	return getDataUnion(vectors, tag_info);
}

DataUnion getDataUnion(const VectorsForData& vectors, const MappingInfo& tag_info)
{
	auto vectors_ = vectors;
	for (auto& el : vectors_.time_values)
	{
		el = vectors.time_values[0];
	}
	auto data = getDdsData(vectors_);
	auto dto = getMediateDataDto(vectors_, tag_info);

	return {data, dto, tag_info};
}

MediateDataDto getMediateDataDto(size_t size, size_t char_size)
{
	auto vectors = VectorsForData(size, char_size);
	auto tag_info = getMappingInfo(size);
	return getMediateDataDto(vectors, tag_info);
}

MediateDataDto getMediateDataDto(const VectorsForData& vectors, const MappingInfo& tag_info)
{
	std::vector<uint32_t> int_tags;
	int_tags.reserve(vectors.int_values.size());
	std::copy_n(tag_info.tags.at(DatasetType::DATA_INT).begin(),
				vectors.int_values.size(),
				std::back_inserter(int_tags));

	std::vector<uint32_t> float_tags;
	float_tags.reserve(vectors.float_values.size());
	std::copy_n(tag_info.tags.at(DatasetType::DATA_FLOAT).begin(),
				vectors.float_values.size(),
				std::back_inserter(float_tags));

	std::vector<uint32_t> double_tags;
	double_tags.reserve(vectors.double_values.size());
	std::copy_n(tag_info.tags.at(DatasetType::DATA_DOUBLE).begin(),
				vectors.double_values.size(),
				std::back_inserter(double_tags));

	std::vector<uint32_t> char_tags;
	char_tags.reserve(vectors.char_qualities.size());
	std::copy_n(tag_info.tags.at(DatasetType::DATA_CHAR).begin(),
				vectors.char_qualities.size(),
				std::back_inserter(char_tags));

	return MediateDataDto{
		vectors.time_service,
		{vectors.time_values, int_tags, vectors.int_values, vectors.int_qualities},
		{vectors.time_values, float_tags, vectors.float_values, vectors.float_qualities},
		{vectors.time_values, double_tags, vectors.double_values, vectors.double_qualities},
		{vectors.time_values,
		 char_tags,
		 getFilledVector(vectors.char_qualities.size(), vectors.char_value),
		 vectors.char_qualities}};
}

template<typename T>
void shuffleVector(std::vector<T>& v)
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(v.begin(), v.end(), g);
}

DataExUnion getDataExUnion(size_t data_ex_size, size_t data_size, size_t char_size)
{
	auto vectors = VectorsForData(data_size, char_size);
	auto ex_vectors = VectorsForData(data_ex_size, char_size);
	auto tag_info = getMappingInfo(data_size > data_ex_size ? data_size : data_ex_size);

	auto data = getDdsData(vectors);
	auto dto = getMediateDataDto(vectors, tag_info);

	auto ex_tag_info = tag_info;
	shuffleVector(ex_tag_info.tags.at(DatasetType::DATA_INT));
	shuffleVector(ex_tag_info.tags.at(DatasetType::DATA_FLOAT));
	shuffleVector(ex_tag_info.tags.at(DatasetType::DATA_DOUBLE));
	shuffleVector(ex_tag_info.tags.at(DatasetType::DATA_CHAR));

	if (data_size > data_ex_size)
	{
		ex_tag_info.tags.at(DatasetType::DATA_INT).resize(data_ex_size);
		ex_tag_info.tags.at(DatasetType::DATA_FLOAT).resize(data_ex_size);
		ex_tag_info.tags.at(DatasetType::DATA_DOUBLE).resize(data_ex_size);
		ex_tag_info.tags.at(DatasetType::DATA_CHAR).resize(data_ex_size);
	}

	auto data_ex = getDdsDataEx(ex_vectors, tag_info);
	auto dto_ex = getMediateDataDto(ex_vectors, tag_info);

	auto result_dto = dto;

	if (data_size < data_ex_size)
	{
		result_dto.data_int.resize(data_ex_size);
		result_dto.data_float.resize(data_ex_size);
		result_dto.data_double.resize(data_ex_size);
		result_dto.data_char.resize(data_ex_size);
	}

	result_dto.time_service = dto_ex.time_service;
	for (int i = 0; i < data_ex_size; ++i)
	{
		auto index = tag_info.tag_to_index.at(DatasetType::DATA_INT)[dto_ex.data_int.id_tag[i]];
		result_dto.data_int.time_source[index] = dto_ex.data_int.time_source[i];
		result_dto.data_int.id_tag[index] = dto_ex.data_int.id_tag[i];
		result_dto.data_int.value[index] = dto_ex.data_int.value[i];
		result_dto.data_int.quality[index] = dto_ex.data_int.quality[i];

		index = tag_info.tag_to_index.at(DatasetType::DATA_FLOAT)[dto_ex.data_float.id_tag[i]];
		result_dto.data_float.time_source[index] = dto_ex.data_float.time_source[i];
		result_dto.data_float.id_tag[index] = dto_ex.data_float.id_tag[i];
		result_dto.data_float.value[index] = dto_ex.data_float.value[i];
		result_dto.data_float.quality[index] = dto_ex.data_float.quality[i];

		index = tag_info.tag_to_index.at(DatasetType::DATA_DOUBLE)[dto_ex.data_double.id_tag[i]];
		result_dto.data_double.time_source[index] = dto_ex.data_double.time_source[i];
		result_dto.data_double.id_tag[index] = dto_ex.data_double.id_tag[i];
		result_dto.data_double.value[index] = dto_ex.data_double.value[i];
		result_dto.data_double.quality[index] = dto_ex.data_double.quality[i];

		index = tag_info.tag_to_index.at(DatasetType::DATA_CHAR)[dto_ex.data_char.id_tag[i]];
		result_dto.data_char.time_source[index] = dto_ex.data_char.time_source[i];
		result_dto.data_char.id_tag[index] = dto_ex.data_char.id_tag[i];
		result_dto.data_char.value[index] = dto_ex.data_char.value[i];
		result_dto.data_char.quality[index] = dto_ex.data_char.quality[i];
	}

	return {data, data_ex, dto, result_dto, tag_info};
}

std::pair<VectorsForData, VectorsForData> divideVectorsForDataToTwo(const VectorsForData& vectors,
																	size_t offset)
{
	VectorsForData first(0);
	first.time_service = vectors.time_service;

	std::copy(vectors.time_values.begin(),
			  vectors.time_values.begin() + offset,
			  std::back_inserter(first.time_values));

	std::copy(vectors.int_values.begin(),
			  vectors.int_values.begin() + offset,
			  std::back_inserter(first.int_values));
	std::copy(vectors.int_qualities.begin(),
			  vectors.int_qualities.begin() + offset,
			  std::back_inserter(first.int_qualities));

	std::copy(vectors.float_values.begin(),
			  vectors.float_values.begin() + offset,
			  std::back_inserter(first.float_values));
	std::copy(vectors.float_qualities.begin(),
			  vectors.float_qualities.begin() + offset,
			  std::back_inserter(first.float_qualities));

	std::copy(vectors.double_values.begin(),
			  vectors.double_values.begin() + offset,
			  std::back_inserter(first.double_values));
	std::copy(vectors.double_qualities.begin(),
			  vectors.double_qualities.begin() + offset,
			  std::back_inserter(first.double_qualities));

	first.char_value = vectors.char_value;
	std::copy(vectors.char_qualities.begin(),
			  vectors.char_qualities.begin() + offset,
			  std::back_inserter(first.char_qualities));

	VectorsForData second(0);
	second.time_service = vectors.time_service;

	std::copy(vectors.time_values.begin() + offset,
			  vectors.time_values.end(),
			  std::back_inserter(second.time_values));

	std::copy(vectors.int_values.begin() + offset,
			  vectors.int_values.end(),
			  std::back_inserter(second.int_values));
	std::copy(vectors.int_qualities.begin() + offset,
			  vectors.int_qualities.end(),
			  std::back_inserter(second.int_qualities));

	std::copy(vectors.float_values.begin() + offset,
			  vectors.float_values.end(),
			  std::back_inserter(second.float_values));
	std::copy(vectors.float_qualities.begin() + offset,
			  vectors.float_qualities.end(),
			  std::back_inserter(second.float_qualities));

	std::copy(vectors.double_values.begin() + offset,
			  vectors.double_values.end(),
			  std::back_inserter(second.double_values));
	std::copy(vectors.double_qualities.begin() + offset,
			  vectors.double_qualities.end(),
			  std::back_inserter(second.double_qualities));

	second.char_value = vectors.char_value;
	std::copy(vectors.char_qualities.begin() + offset,
			  vectors.char_qualities.end(),
			  std::back_inserter(second.char_qualities));

	return {first, second};
}

std::pair<MappingInfo, MappingInfo> divideMappingInfoToTwo(const MappingInfo& info, size_t offset)
{
	MappingInfo first = info;
	MappingInfo second;

	first.offset = 0;
	second.offset = offset;

	for (auto& [type, tags] : first.tags)
	{
		tags.resize(offset);
	}
	for (auto& [type, tags] : info.tags)
	{
		std::copy(tags.begin() + offset, tags.end(), std::back_inserter(second.tags[type]));
	}

	second.tag_to_index = info.tag_to_index;
	for (auto& [type, first_tag_to_index] : first.tag_to_index)
	{
		for (const auto& second_tag : second.tags[type])
		{
			first_tag_to_index.erase(second_tag);
		}
	}
	for (auto& [type, second_tag_to_index] : second.tag_to_index)
	{
		for (const auto& first_tag : first.tags[type])
		{
			second_tag_to_index.erase(first_tag);
		}
	}

	return {first, second};
}

template<>
void sortByTimeService(std::vector<MediateDataDto>& vector_of_datasets)
{
	std::sort(vector_of_datasets.begin(),
			  vector_of_datasets.end(),
			  [](const MediateDataDto& a, const MediateDataDto& b) {
				  return a.time_service < b.time_service;
			  });
}

template<>
void sortByTimeService(std::vector<MediateAlarmDto>& vector_of_datasets)
{
	std::sort(vector_of_datasets.begin(),
			  vector_of_datasets.end(),
			  [](const MediateAlarmDto& a, const MediateAlarmDto& b) {
				  return a.time_service < b.time_service;
			  });
}

bool OneTestConditions::operator==(const OneTestConditions& rhs) const
{
	return rhs.all_vectors_sizes == all_vectors_sizes && rhs.char_vector_sizes == char_vector_sizes
		   && rhs.publication_interval == publication_interval;
}

bool operator==(const GlobalTestConditions& lhs, const GlobalTestConditions& rhs)
{
	return lhs.isSync == rhs.isSync && lhs.isWsServerRun == rhs.isWsServerRun
		   && lhs.isDdsServerRun == rhs.isDdsServerRun && lhs.ip == rhs.ip
		   && std::equal(lhs.conditions.begin(), lhs.conditions.end(), rhs.conditions.begin())
		   && lhs.samples_number == rhs.samples_number;
}

GlobalTestConditions parseJsonToGlobalTestConditions(const nlohmann::json& json)
{
	GlobalTestConditions conditions;
	conditions.ip = json["ip"].get<std::string>();
	conditions.isSync = json["isSync"].get<bool>();
	conditions.isWsServerRun = json["isWsServerRun"].get<bool>();
	conditions.isDdsServerRun = json["isDdsServerRun"].get<bool>();
	conditions.samples_number = json["samples_number"].get<size_t>();
	for (const auto& c : json["conditions"])
	{
		OneTestConditions condition{
			c["all_vectors_sizes"].get<size_t>(),
			c["char_vector_sizes"].get<size_t>(),
			c["publication_interval"].get<int64_t>(),
		};
		conditions.conditions.push_back(condition);
	}
	return conditions;
}

std::string formMappingTestName(std::string description, const OneTestConditions& cond)
{
	return "\n" + description + " v = " + std::to_string(cond.all_vectors_sizes)
		   + " cv = " + std::to_string(cond.char_vector_sizes);
}
} // namespace scada_ate::dds