#include "../DataTransferService/DdsService/DdsSubscriber.h"
#include "../DataTransferService/WsService/WsServer.h"
#include <gtest/gtest.h>
//#include "../include/test_runner.h"

std::vector<SubscriberConfig> configs({
        {0, 10000, "DDSData1", "DDSData", TopicType::DDS_DATA, 2, 1000},
        {1, 10000, "DDSData2", "DDSData", TopicType::DDS_DATA, 2, 1000}
    });

ServiceConfig config({
    "Participant_sub",
    "127.0.0.1",
    4042,
    {"127.0.0.1"},
    configs,
    1000,
    1000,
    1000,
    1000,
    1000,
    1000,
    1000,
    1000,
    1000,
    1000
    });


void TestDataTransition()
{
    oatpp::base::Environment::init();

    Configure conf;
    WebsockServer server(conf);
    SubscriberService* mysub = new SubscriberService(config, &server);

    std::thread tcp_thread([](SubscriberService* mysub) {
        mysub->setVectorSizesInDataTopic();
        if (mysub->initSubscribers())
        {
            mysub->runSubscribers();
        }
        }, mysub);

    server.run();

    delete mysub;
    oatpp::base::Environment::destroy();
}

TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

template<class T>
std::vector<T> getVector(size_t size = 0, T offset = 0)
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

template<class T>
std::vector<T> fillVector(T elem, size_t size)
{
    std::vector<T> result(size, elem);
    return result;
}

std::pair<DDSData, DataDto> getEqualDdsData(size_t size = 4)
{
    DDSData data1;

    data1.time_service(100);
    data1.time_source(101);
    data1.data_int().value(getVector<int32_t>(size, 1));
    data1.data_int().quality(fillVector('a', size));
    data1.data_float().value(getVector<float>(size, 1));
    data1.data_float().quality(fillVector('a', size));
    data1.data_double().value(getVector<double>(size, 1));
    data1.data_double().quality(fillVector('a', size));

    DataChar data_char;
    data_char.value(fillVector('a', size));
    data1.data_char().value(fillVector(data_char, size));
    data1.data_char().quality(fillVector('a', size));

    DataDto	dto{
        100,
        {
            fillVector<int64_t>(101, size),
            getVector<int32_t>(size, 1),
            fillVector('a', size)
        },
        {
            fillVector<int64_t>(101, size),
            getVector<float>(size, 1),
            fillVector('a', size)
        },
        {
            fillVector<int64_t>(101, size),
            getVector<double>(size, 1),
            fillVector('a', size)
        },
        {
            fillVector<int64_t>(101, size),
            fillVector(fillVector('a', size), size),
            fillVector('a', size)
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
    data_ex_char.value(fillVector('a', 4));
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
    data2.data_int().value(getVector<int32_t>(size, 1));
    data2.data_int().quality(fillVector('a', size));
    data2.data_float().value(getVector<float>(size, 1));
    data2.data_float().quality(fillVector('a', size));
    data2.data_double().value(getVector<double>(size, 1));
    data2.data_double().quality(fillVector('a', size));

    DataChar data_char;
    data_char.value(fillVector('a', size));
    data2.data_char().value(fillVector(data_char, size));
    data2.data_char().quality(fillVector('a', size));

    DataDto	dto{
        100,
        {
            fillVector<int64_t>(101, 4),
            {0, 0, 2, 3},
            {'b', 'b', 'a', 'a'}
        },
        {
            fillVector<int64_t>(101, 4),
            {0, 0, 2, 3},
            {'b', 'b', 'a', 'a'}
        },
        {
            fillVector<int64_t>(101, 4),
            {0, 0, 2, 3},
            {'b', 'b', 'a', 'a'}
        },
        {
            {102, 102, 101, 101},
            fillVector(fillVector('a', 4), 4),
            {'b', 'b', 'a', 'a'}
        }
    };

    DataDto	dto_to_change{
        100,
        {
            fillVector<int64_t>(101, 4),
            getVector<int32_t>(4, 1),
            fillVector('a', 4)
        },
        {
            fillVector<int64_t>(101, 4),
            getVector<float>(4, 1),
            fillVector('a', 4)
        },
        {
            fillVector<int64_t>(101, 4),
            getVector<double>(4, 1),
            fillVector('a', 4)
        },
        {
            fillVector<int64_t>(101, 4),
            fillVector(fillVector('a', 4), 4),
            fillVector('a', 4)
        }
    };

    return {data2, data1, dto_to_change, dto};
}

TEST(DtoTest, DdsDataConvertation) {
    auto dds_data = getEqualDdsData(4);
    DataMapper mapper;
    DataDto d = mapper.mapDdsData(std::move(dds_data.first));
    EXPECT_EQ(dds_data.second, d);

    DataExUnion data_ex_union = getEqualDdsDataEx(2);
    DataDto d2 = mapper.mapDdsDataEx(data_ex_union.data_ex, data_ex_union.dto_to_change);
    EXPECT_EQ(d2, data_ex_union.dto);
}


int main(int argc, char* argv[])
{
    //TestDataTransition();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}