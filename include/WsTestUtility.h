#ifndef WS_TEST_UTILITY_H_
#define WS_TEST_UTILITY_H_

#include "TestUtility.h"
#include "WsDto.h"

struct WsDataUnion
{
    DDSData dds_data;
    DataDto data_dto;
    WsDataDto::Wrapper ws_dto;
};

WsDataUnion getWsDataUnion(size_t size = 4)
{
    WsDataUnion data_union;
    auto buf = getEqualDdsData(size);
    data_union.dds_data = buf.first;
    data_union.data_dto = buf.second;

    auto collect_int = WsDataCollectionInt::createShared();
    auto collect_float = WsDataCollectionFloat::createShared();
    auto collect_double = WsDataCollectionDouble::createShared();
    auto collect_char = WsDataCollectionChar::createShared();

    AdditionalTopicInfo info = getAdditionalTopicInfo(size);
    std::string quality(size, 'a');

    collect_int->quality = quality.c_str();
    collect_float->quality = quality.c_str();
    collect_double->quality = quality.c_str();
    collect_char->quality = quality.c_str();

    collect_int->time_source = {};
    collect_int->id_tag = {};
    collect_int->value = {};

    collect_float->time_source = {};
    collect_float->id_tag = {};
    collect_float->value = {};

    collect_double->time_source = {};
    collect_double->id_tag = {};
    collect_double->value = {};

    collect_char->time_source = {};
    collect_char->id_tag = {};
    collect_char->value = {};

    for (int i = 0; i < size; ++i)
    {
        collect_int->time_source->push_back(oatpp::Int64(101));
        collect_int->id_tag->push_back(oatpp::UInt32(info.tags.at(DataCollectiionType::DATA_INT)[i]));
        collect_int->value->push_back(oatpp::Int32(1));
        
        collect_float->time_source->push_back(oatpp::Int64(101));
        collect_float->id_tag->push_back(oatpp::UInt32(info.tags.at(DataCollectiionType::DATA_FLOAT)[i]));
        collect_float->value->push_back(oatpp::Float32(1));
        
        collect_double->time_source->push_back(oatpp::Int64(101));
        collect_double->id_tag->push_back(oatpp::UInt32(info.tags.at(DataCollectiionType::DATA_DOUBLE)[i]));
        collect_double->value->push_back(oatpp::Float64(1));

        collect_char->time_source->push_back(oatpp::Int64(101));
        collect_char->id_tag->push_back(oatpp::UInt32(info.tags.at(DataCollectiionType::DATA_CHAR)[i]));
        collect_char->value->push_back(oatpp::String(quality.c_str()));
    }

    data_union.ws_dto = WsDataDto::createShared();

    data_union.ws_dto->time_service = 100;
    data_union.ws_dto->data_collection_int = collect_int;
    data_union.ws_dto->data_collection_float = collect_float;
    data_union.ws_dto->data_collection_double = collect_double;
    data_union.ws_dto->data_collection_char = collect_char;

    return data_union;
}

#endif //!WS_TEST_UTILITY_H_