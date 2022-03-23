#ifndef TEST_UTILITY_H_
#define TEST_UTILITY_H_

#include "../TypeTopicsDDS/TypeTopicsPubSubTypes.h"
#include "CommonClasses.h"

template<class T>
std::vector<T> getDefaultVector(size_t size = 0, T offset = 0)
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

std::vector<char> getDefaultVector(size_t size = 0, char offset = 0)
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

std::pair<DDSData, DataDto> getEqualDdsData(size_t size = 4)
{
    DDSData data1;

    data1.time_service(100);
    data1.time_source(101);

    data1.data_int().value(getDefaultVector<int32_t>(size, 1));
    data1.data_int().quality(getDefaultVector(size));

    data1.data_float().value(getDefaultVector<float>(size, 1));
    data1.data_float().quality(getDefaultVector(size));

    data1.data_double().value(getDefaultVector<double>(size, 1));
    data1.data_double().quality(getDefaultVector(size));

    DataChar data_char;
    data_char.value(getFilledVector(size, 'a'));
    data1.data_char().value(getFilledVector(size, data_char));
    data1.data_char().quality(getDefaultVector(size));

    DataDto	dto{
        100,
        {
            getFilledVector<int64_t>(size, 101),
            getDefaultVector<int32_t>(size, 1),
            getDefaultVector(size)
        },
        {
            getFilledVector<int64_t>(size, 101),
            getDefaultVector<float>(size, 1),
            getDefaultVector(size)
        },
        {
            getFilledVector<int64_t>(size, 101),
            getDefaultVector<double>(size, 1),
            getDefaultVector(size)
        },
        {
            getFilledVector<int64_t>(size, 101),
            getFilledVector(size, getFilledVector(size, 'a')),
            getDefaultVector(size)
        }
    };

    return std::make_pair(data1, dto);
}

struct DataExUnion
{
    DDSData data_;
    DDSDataEx data_ex;
    DataDto dto_to_change;
    DataDto dto;
};

DataExUnion getEqualDdsDataEx(size_t size_ex = 2, size_t size_data = 4)
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
        data_ex_int.id_tag(i);
        data_ex_float.id_tag(i);
        data_ex_double.id_tag(i);
        data_ex_char.id_tag(i);
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

    DataDto	dto_to_change{
        100,
        {
            getFilledVector<int64_t>(size_data, 101),
            getDefaultVector<int32_t>(size_data, 1),
            getDefaultVector(size_data)
        },
        {
            getFilledVector<int64_t>(size_data, 101),
            getDefaultVector<float>(size_data, 1),
            getDefaultVector(size_data)
        },
        {
            getFilledVector<int64_t>(size_data, 101),
            getDefaultVector<double>(size_data, 1),
            getDefaultVector(size_data)
        },
        {
            getFilledVector<int64_t>(size_data, 101),
            getFilledVector(size_data, getFilledVector(size_data, 'a')),
            getDefaultVector(size_data)
        }
    };

    DataDto	result_dto = dto_to_change;

    if (size_data < size_ex)
    {
        result_dto.data_int.time_source.resize(size_ex);
        result_dto.data_int.value.resize(size_ex);
        result_dto.data_int.quality.resize(size_ex);

        result_dto.data_float.time_source.resize(size_ex);
        result_dto.data_float.value.resize(size_ex);
        result_dto.data_float.quality.resize(size_ex);

        result_dto.data_double.time_source.resize(size_ex);
        result_dto.data_double.value.resize(size_ex);
        result_dto.data_double.quality.resize(size_ex);

        result_dto.data_char.time_source.resize(size_ex);
        result_dto.data_char.value.resize(size_ex);
        result_dto.data_char.quality.resize(size_ex);
    }

    for (int i = 0; i < size_ex; ++i)
    {
        result_dto.data_int.time_source[i] = data_ex_int.time_source();
        result_dto.data_int.value[i] = data_ex_int.value();
        result_dto.data_int.quality[i] = data_ex_int.quality();

        result_dto.data_float.time_source[i] = data_ex_float.time_source();
        result_dto.data_float.value[i] = data_ex_float.value();
        result_dto.data_float.quality[i] = data_ex_float.quality();

        result_dto.data_double.time_source[i] = data_ex_double.time_source();
        result_dto.data_double.value[i] = data_ex_double.value();
        result_dto.data_double.quality[i] = data_ex_double.quality();

        result_dto.data_char.time_source[i] = data_ex_char.time_source();
        result_dto.data_char.value[i] = data_ex_char.value();
        result_dto.data_char.quality[i] = data_ex_char.quality();
    }


    return { data2, data1, dto_to_change, result_dto };
}


#endif //!TEST_UTILITY_H_