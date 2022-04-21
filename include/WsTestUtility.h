#ifndef WS_TEST_UTILITY_H_
#define WS_TEST_UTILITY_H_

#include "TestUtility.h"
#include "WsDto.h"

struct WsDataUnion
{
    DDSData dds_data;
    MediateDataDto data_dto;
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

    for (int i = 0; i < size; ++i)
    {
        collect_int->tsrc->push_back(oatpp::Int64(101));
        collect_int->tag->push_back(oatpp::UInt32(info.tags.at(DataCollectiionType::DATA_INT)[i]));
        collect_int->val->push_back(oatpp::Int32(1));
        
        collect_float->tsrc->push_back(oatpp::Int64(101));
        collect_float->tag->push_back(oatpp::UInt32(info.tags.at(DataCollectiionType::DATA_FLOAT)[i]));
        collect_float->val->push_back(oatpp::Float32(1));
        
        collect_double->tsrc->push_back(oatpp::Int64(101));
        collect_double->tag->push_back(oatpp::UInt32(info.tags.at(DataCollectiionType::DATA_DOUBLE)[i]));
        collect_double->val->push_back(oatpp::Float64(1));

        collect_char->tsrc->push_back(oatpp::Int64(101));
        collect_char->tag->push_back(oatpp::UInt32(info.tags.at(DataCollectiionType::DATA_CHAR)[i]));
        collect_char->val->push_back(oatpp::String(quality.c_str()));
    }

    data_union.ws_dto = WsDataDto::createShared();

    data_union.ws_dto->tsrv = 100;
    data_union.ws_dto->di = collect_int;
    data_union.ws_dto->df = collect_float;
    data_union.ws_dto->dd = collect_double;
    data_union.ws_dto->dc = collect_char;

    return data_union;
}

#endif //!WS_TEST_UTILITY_H_