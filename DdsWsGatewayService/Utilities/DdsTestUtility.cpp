#include "DdsTestUtility.h"

template<class T>
std::vector<T> getDefaultVector(size_t size, T offset)
{
	std::vector<T> result(size);
	if (offset != 0)
	{
		for (size_t i = 1; i < size; ++i)
		{
			result[i] = result[i - 1] + offset;
		}
	}
	return result;
}

std::vector<char> getDefaultVector(size_t size, char offset)
{
	std::vector<char> result(size, 'a');
	return result;
}

template<class T>
std::vector<T> getFilledVector(size_t size, T filler)
{
	std::vector<T> result(size, filler);
	return result;
}

AdditionalTopicInfo getAdditionalTopicInfo(size_t size)
{
	AdditionalTopicInfo info;
	info.tag_to_index[DataCollectiionType::DATA_INT];
	info.tag_to_index[DataCollectiionType::DATA_FLOAT];
	info.tag_to_index[DataCollectiionType::DATA_DOUBLE];
	info.tag_to_index[DataCollectiionType::DATA_CHAR];
	info.tags[DataCollectiionType::DATA_INT];
	info.tags[DataCollectiionType::DATA_FLOAT];
	info.tags[DataCollectiionType::DATA_DOUBLE];
	info.tags[DataCollectiionType::DATA_CHAR];
	for (auto& [data_type, tag_to_index] : info.tag_to_index)
	{
		info.tags.at(data_type).reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			tag_to_index[1000000 + i] = i;
			info.tags.at(data_type).push_back(1000000 + i);
		}
	}
	info.topic_name = "Name";
	return info;
}

DDSData getDdsData(size_t size, size_t char_size)
{
	DDSData data1;

	data1.time_service(100);
	data1.time_source(101);

	data1.data_int().value(getDefaultVector<int32_t>(size));
	data1.data_int().quality(getDefaultVector(size));

	data1.data_float().value(getDefaultVector<float>(size));
	data1.data_float().quality(getDefaultVector(size));

	data1.data_double().value(getDefaultVector<double>(size));
	data1.data_double().quality(getDefaultVector(size));

	DataChar data_char;
	data_char.value(getFilledVector(char_size, 'a'));
	data1.data_char().value(getFilledVector(size, data_char));
	data1.data_char().quality(getDefaultVector(size));

	return data1;
}

DDSDataEx getDdsDataEx(size_t size, size_t char_size)
{
	DDSDataEx data1;
	data1.time_service(100);
	DataExInt data_ex_int;
	data_ex_int.time_source(101);
	data_ex_int.value(0);
	data_ex_int.quality('b');
	DataExFloat data_ex_float;
	data_ex_float.time_source(101);
	data_ex_float.value(0);
	data_ex_float.quality('b');
	DataExDouble data_ex_double;
	data_ex_double.time_source(101);
	data_ex_double.value(0);
	data_ex_double.quality('b');
	DataExChar data_ex_char;
	data_ex_char.time_source(102);
	data_ex_char.value(getDefaultVector(char_size));
	data_ex_char.quality('b');

	for (int i = 0; i < size; ++i)
	{
		data_ex_int.id_tag(1000000 + i);
		data_ex_float.id_tag(1000000 + i);
		data_ex_double.id_tag(1000000 + i);
		data_ex_char.id_tag(1000000 + i);
		data1.data_int().push_back(data_ex_int);
		data1.data_float().push_back(data_ex_float);
		data1.data_double().push_back(data_ex_double);
		data1.data_char().push_back(data_ex_char);
	}

	return data1;
}

std::pair<DDSData, MediateDataDto> getEqualDdsData(size_t size)
{
	DDSData data1;

	data1.time_service(100);
	data1.time_source(101);

	data1.data_int().value(getFilledVector<int32_t>(size, 1));
	data1.data_int().quality(getFilledVector(size, 'a'));

	data1.data_float().value(getFilledVector<float>(size, 1));
	data1.data_float().quality(getFilledVector(size, 'a'));

	data1.data_double().value(getFilledVector<double>(size, 1));
	data1.data_double().quality(getFilledVector(size, 'a'));

	DataChar data_char;
	data_char.value(getFilledVector(size, 'a'));
	data1.data_char().value(getFilledVector(size, data_char));
	data1.data_char().quality(getFilledVector(size, 'a'));

	MediateDataDto dto{100,
					   {getFilledVector<int64_t>(size, 101),
						getAdditionalTopicInfo(size).tags.at(DataCollectiionType::DATA_INT),
						getFilledVector<int32_t>(size, 1),
						getFilledVector(size, 'a')},
					   {getFilledVector<int64_t>(size, 101),
						getAdditionalTopicInfo(size).tags.at(DataCollectiionType::DATA_FLOAT),
						getFilledVector<float>(size, 1),
						getFilledVector(size, 'a')},
					   {getFilledVector<int64_t>(size, 101),
						getAdditionalTopicInfo(size).tags.at(DataCollectiionType::DATA_DOUBLE),
						getFilledVector<double>(size, 1),
						getFilledVector(size, 'a')},
					   {getFilledVector<int64_t>(size, 101),
						getAdditionalTopicInfo(size).tags.at(DataCollectiionType::DATA_CHAR),
						getFilledVector(size, getFilledVector(size, 'a')),
						getFilledVector(size, 'a')}};

	return std::make_pair(data1, dto);
}

DataExUnion getEqualDdsDataEx(size_t size_ex, size_t size_data)
{
	DDSDataEx data1;
	data1.time_service(100);
	DataExInt data_ex_int;
	data_ex_int.time_source(101);
	data_ex_int.value(0);
	data_ex_int.quality('b');
	DataExFloat data_ex_float;
	data_ex_float.time_source(101);
	data_ex_float.value(0);
	data_ex_float.quality('b');
	DataExDouble data_ex_double;
	data_ex_double.time_source(101);
	data_ex_double.value(0);
	data_ex_double.quality('b');
	DataExChar data_ex_char;
	data_ex_char.time_source(102);
	data_ex_char.value(getDefaultVector(size_ex));
	data_ex_char.quality('b');

	for (int i = 0; i < size_ex; ++i)
	{
		data_ex_int.id_tag(1000000 + i);
		data_ex_float.id_tag(1000000 + i);
		data_ex_double.id_tag(1000000 + i);
		data_ex_char.id_tag(1000000 + i);
		data1.data_int().push_back(data_ex_int);
		data1.data_float().push_back(data_ex_float);
		data1.data_double().push_back(data_ex_double);
		data1.data_char().push_back(data_ex_char);
	}

	DDSData data2;
	data2.time_service(100);
	data2.data_int().value(getDefaultVector<int32_t>(size_ex, 1));
	data2.data_int().quality(getDefaultVector(size_ex));
	data2.data_float().value(getDefaultVector<float>(size_ex, 1));
	data2.data_float().quality(getDefaultVector(size_ex));
	data2.data_double().value(getDefaultVector<double>(size_ex, 1));
	data2.data_double().quality(getDefaultVector(size_ex));

	DataChar data_char;
	data_char.value(getFilledVector(size_ex, 'a'));
	data2.data_char().value(getFilledVector(size_ex, data_char));
	data2.data_char().quality(getDefaultVector(size_ex));

	MediateDataDto dto_to_change{
		100,
		{getFilledVector<int64_t>(size_data, 101),
		 getAdditionalTopicInfo(size_data).tags.at(DataCollectiionType::DATA_INT),
		 getDefaultVector<int32_t>(size_data, 1),
		 getDefaultVector(size_data)},
		{getFilledVector<int64_t>(size_data, 101),
		 getAdditionalTopicInfo(size_data).tags.at(DataCollectiionType::DATA_FLOAT),
		 getDefaultVector<float>(size_data, 1),
		 getDefaultVector(size_data)},
		{getFilledVector<int64_t>(size_data, 101),
		 getAdditionalTopicInfo(size_data).tags.at(DataCollectiionType::DATA_DOUBLE),
		 getDefaultVector<double>(size_data, 1),
		 getDefaultVector(size_data)},
		{getFilledVector<int64_t>(size_data, 101),
		 getAdditionalTopicInfo(size_data).tags.at(DataCollectiionType::DATA_CHAR),
		 getFilledVector(size_data, getFilledVector(size_data, 'a')),
		 getDefaultVector(size_data)}};

	MediateDataDto result_dto = dto_to_change;

	if (size_data < size_ex)
	{
		result_dto.data_int.time_source.resize(size_ex);
		result_dto.data_int.id_tag.resize(size_ex);
		result_dto.data_int.value.resize(size_ex);
		result_dto.data_int.quality.resize(size_ex);

		result_dto.data_float.time_source.resize(size_ex);
		result_dto.data_float.id_tag.resize(size_ex);
		result_dto.data_float.value.resize(size_ex);
		result_dto.data_float.quality.resize(size_ex);

		result_dto.data_double.time_source.resize(size_ex);
		result_dto.data_double.id_tag.resize(size_ex);
		result_dto.data_double.value.resize(size_ex);
		result_dto.data_double.quality.resize(size_ex);

		result_dto.data_char.time_source.resize(size_ex);
		result_dto.data_char.id_tag.resize(size_ex);
		result_dto.data_char.value.resize(size_ex);
		result_dto.data_char.quality.resize(size_ex);
	}

	for (int i = 0; i < size_ex; ++i)
	{
		result_dto.data_int.time_source[i] = data_ex_int.time_source();
		result_dto.data_int.id_tag[i] = 1000000 + i;
		result_dto.data_int.value[i] = data_ex_int.value();
		result_dto.data_int.quality[i] = data_ex_int.quality();

		result_dto.data_float.time_source[i] = data_ex_float.time_source();
		result_dto.data_float.id_tag[i] = 1000000 + i;
		result_dto.data_float.value[i] = data_ex_float.value();
		result_dto.data_float.quality[i] = data_ex_float.quality();

		result_dto.data_double.time_source[i] = data_ex_double.time_source();
		result_dto.data_double.id_tag[i] = 1000000 + i;
		result_dto.data_double.value[i] = data_ex_double.value();
		result_dto.data_double.quality[i] = data_ex_double.quality();

		result_dto.data_char.time_source[i] = data_ex_char.time_source();
		result_dto.data_char.id_tag[i] = 1000000 + i;
		result_dto.data_char.value[i] = data_ex_char.value();
		result_dto.data_char.quality[i] = data_ex_char.quality();
	}

	return {data2, data1, dto_to_change, result_dto};
}

bool OneTestConditions::operator==(const OneTestConditions& rhs) const
{
	return rhs.all_vectors_sizes == all_vectors_sizes && rhs.char_vector_sizes == char_vector_sizes
		   && rhs.samples_number == samples_number
		   && rhs.publication_interval == publication_interval;
}

bool operator==(const GlobalTestConditions& lhs, const GlobalTestConditions& rhs)
{
	return lhs.isSync == rhs.isSync && lhs.isWsServerRun == rhs.isWsServerRun
		   && lhs.isDdsServerRun == rhs.isDdsServerRun && lhs.ip == rhs.ip
		   && std::equal(lhs.conditions.begin(), lhs.conditions.end(), rhs.conditions.begin());
}

GlobalTestConditions parseJsonToGlobalTestConditions(const nlohmann::json& json)
{
	GlobalTestConditions conditions;
	conditions.ip = json["ip"].get<std::string>();
	conditions.isSync = json["isSync"].get<bool>();
	conditions.isWsServerRun = json["isWsServerRun"].get<bool>();
	conditions.isDdsServerRun = json["isDdsServerRun"].get<bool>();
	for (const auto& c : json["conditions"])
	{
		OneTestConditions condition{
			c["all_vectors_sizes"].get<size_t>(),
			c["char_vector_sizes"].get<size_t>(),
			c["samples_number"].get<size_t>(),
			c["publication_interval"].get<uint64_t>(),
		};
		conditions.conditions.push_back(condition);
	}
	return conditions;
}
