#include <utility>

#include "DataCacher.h"

MediateDataDto DdsTopicToMediateDtoMapper::mapDdsData(DDSData data, const AdditionalTopicInfo& info)
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

MediateDataDto DdsTopicToMediateDtoMapper::mapDdsDataEx(MediateDataDto prev_dto,
														const DDSDataEx& cur_data_ex,
														const AdditionalTopicInfo& info)
{
	prev_dto.time_service = cur_data_ex.time_service();
	fillChanged(prev_dto.data_int,
				cur_data_ex.data_int(),
				info.tag_to_index.at(DataCollectiionType::DATA_INT));
	fillChanged(prev_dto.data_float,
				cur_data_ex.data_float(),
				info.tag_to_index.at(DataCollectiionType::DATA_FLOAT));
	fillChanged(prev_dto.data_double,
				cur_data_ex.data_double(),
				info.tag_to_index.at(DataCollectiionType::DATA_DOUBLE));
	fillChanged(prev_dto.data_char,
				cur_data_ex.data_char(),
				info.tag_to_index.at(DataCollectiionType::DATA_DOUBLE));

	return prev_dto;
}

MediateAlarmDto DdsTopicToMediateDtoMapper::mapDdsAlarm(DDSAlarm data,
														const AdditionalTopicInfo& info)
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

MediateAlarmDto DdsTopicToMediateDtoMapper::mapDdsAlarmEx(MediateAlarmDto prev_dto,
														  const DDSAlarmEx& cur_data_ex,
														  const AdditionalTopicInfo& info)
{
	prev_dto.time_service = cur_data_ex.time_service();
	fillChanged(
		prev_dto, cur_data_ex.alarms(), info.tag_to_index.at(DataCollectiionType::ALARM_UINT32));
	return prev_dto;
}

template <class DtoDataCollection, class DdsSample>
void DdsTopicToMediateDtoMapper::fillChanged(DtoDataCollection& prev_dto_collection,
											 const std::vector<DdsSample>& cur_samples,
											 const TagToIndex& tag_to_index)
{
	size_t n = cur_samples.size();
	for (const auto& sample : cur_samples)
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

WsDataDto::Wrapper MediateDtoToWsDtoMapper::mapDataDto(const MediateDataDto& data)
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

template <class OatppT, class T>
void MediateDtoToWsDtoMapper::fillVector(oatpp::Vector<OatppT>& oatpp_v, const std::vector<T>& v)
{
	oatpp_v = {};
	oatpp_v->reserve(v.size());
	for (T elem : v)
	{
		oatpp_v->push_back(OatppT(elem));
	}
}

void MediateDtoToWsDtoMapper::fillVector(oatpp::Vector<oatpp::String>& oatpp_v,
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

void MediateDtoToWsDtoMapper::fillVector(oatpp::String& oatpp_v, const std::vector<char>& v)
{
	std::string str(v.begin(), v.end());
	oatpp_v = oatpp::String(str.c_str());
}

DataCacher::DataCacher(size_t depth)
	: depth_(depth)
{
	analyser_ = PackageAnalyser::getInstance();
	analyser_->addDataToAnalyse("DDSData to Dto");
	analyser_->addDataToAnalyse("DDSDataEx to Dto");
}

void DataCacher::cache(DDSData data, const AdditionalTopicInfo& info)
{
	auto start = TimeConverter::GetTime_LLmcs();

	data_cache_.push_back(mapper_.mapDdsData(std::move(data), info));

	analyser_->pushDataTimestamp("DDSData to Dto", TimeConverter::GetTime_LLmcs() - start);

	std::lock_guard<std::mutex> guard(std::mutex());
	if (data_cache_.size() > depth_)
	{
		data_cache_.pop_front();
	}
}

void DataCacher::cache(DDSData data,
					   const AdditionalTopicInfo& info,
					   const AdditionalPackageInfo& package_info)
{
	data_cache_.push_back(mapper_.mapDdsData(std::move(data), info));

	std::lock_guard<std::mutex> guard(std::mutex());
	if (data_cache_.size() > depth_)
	{
		data_cache_.pop_front();
	}
}

void DataCacher::cache(const DDSDataEx& data, const AdditionalTopicInfo& info)
{
	auto start = TimeConverter::GetTime_LLmcs();

	if (!data_cache_.empty())
	{
		data_cache_.push_back(mapper_.mapDdsDataEx(data_cache_.back().value(), data, info));
	}
	else
	{
		//TODO MediateDataDto default constructor check
		data_cache_.push_back(mapper_.mapDdsDataEx(MediateDataDto(), data, info));
	}

	analyser_->pushDataTimestamp("DDSDataEx to Dto", TimeConverter::GetTime_LLmcs() - start);

	std::lock_guard<std::mutex> guard(std::mutex());
	if (data_cache_.size() > depth_)
	{
		data_cache_.pop_front();
	}
}

std::optional<MediateDataDto> DataCacher::popDdsDto()
{
	return data_cache_.pop_front();
}

void DataCacher::cache(DDSAlarm data, const AdditionalTopicInfo& info) { }

void DataCacher::cache(const DDSAlarmEx& data, const AdditionalTopicInfo& info) { }

std::deque<MediateDataDto> DataCacher::getDataCacheCopy() const
{
	return data_cache_.getDequeCopy();
}