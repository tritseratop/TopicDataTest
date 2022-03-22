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

DataExUnion getEqualDdsDataEx(size_t size = 2)
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
    data_ex_char.value(getDefaultVector(4));
    data_ex_char.quality('b');

    for (int i = 0; i < size; ++i)
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
    data2.data_int().value(getDefaultVector<int32_t>(size, 1));
    data2.data_int().quality(getDefaultVector(size));
    data2.data_float().value(getDefaultVector<float>(size, 1));
    data2.data_float().quality(getDefaultVector(size));
    data2.data_double().value(getDefaultVector<double>(size, 1));
    data2.data_double().quality(getDefaultVector(size));

    DataChar data_char;
    data_char.value(getFilledVector(size, 'a'));
    data2.data_char().value(getFilledVector(size, data_char));
    data2.data_char().quality(getDefaultVector(size));

    DataDto	dto{
        100,
        {
            getFilledVector<int64_t>(4, 101),
            {0, 0, 2, 3},
            {'b', 'b', 'a', 'a'}
        },
        {
            getFilledVector<int64_t>(4, 101),
            {0, 0, 2, 3},
            {'b', 'b', 'a', 'a'}
        },
        {
            getFilledVector<int64_t>(4, 101),
            {0, 0, 2, 3},
            {'b', 'b', 'a', 'a'}
        },
        {
            {102, 102, 101, 101},
            getFilledVector(4, getFilledVector(4, 'a')),
            {'b', 'b', 'a', 'a'}
        }
    };

    DataDto	dto_to_change{
        100,
        {
            getFilledVector<int64_t>(4, 101),
            getDefaultVector<int32_t>(4, 1),
            getDefaultVector(4)
        },
        {
            getFilledVector<int64_t>(4, 101),
            getDefaultVector<float>(4, 1),
            getDefaultVector(4)
        },
        {
            getFilledVector<int64_t>(4, 101),
            getDefaultVector<double>(4, 1),
            getDefaultVector(4)
        },
        {
            getFilledVector<int64_t>(4, 101),
            getFilledVector(4, getFilledVector(4, 'a')),
            getDefaultVector(4)
        }
    };

    return { data2, data1, dto_to_change, dto };
}


#endif //!TEST_UTILITY_H_