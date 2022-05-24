#include "Lib/DdsService/DdsSubscriber.h"
#include "Lib/Notifier/Notifier.h"
#include "Utilities/DdsTestUtility.h"
#include "Utilities/WsTestUtility.h"

int main(int argc, char* argv[])
{
	std::ifstream ifile("test.json");
	nlohmann::json json;
	ifile >> json;
	GlobalTestConditions conditions = parseJsonToGlobalTestConditions(json);

	auto configs = createDdsServiceConfigs(conditions);

	bool isWsRun = conditions.isWsServerRun;
	bool isDdsRun = conditions.isDdsServerRun;

	if (isDdsRun && isWsRun)
	{
		oatpp::base::Environment::init();
		{
			Configure ws_conf;
			DataCacher cacher(5);
			WebsockServer server(ws_conf, cacher);

			SubscriberService* mysub = new SubscriberService({}, cacher);

			std::thread dds_service_thread([&]() {
				int i = 1;
				for (auto conf : configs)
				{
					Notifier notifier(server, cacher);
					std::cout << std::to_string(i++) + ". SERVICE RUN WITH TOPIC "
									 + conf.configs[0].topic_type_name
									 + " size: " + std::to_string(conf.configs[0].vector_size)
							  << std::endl;
					mysub->changeSubscribersConfig(conf);
					std::thread ws_thread(
						[&notifier, &conf]() { notifier.runNotifying(conf.ws_data_sleep); });
					mysub->runSubscribers();
					notifier.stopNotifying();
					ws_thread.join();
				}
				server.stop();
			});

			server.run();
			std::cout << "ws_server stopped" << std::endl;

			delete mysub;

			if (dds_service_thread.joinable())
			{
				dds_service_thread.join();
			}
		}
		oatpp::base::Environment::destroy();
	}
	else if (isDdsRun && !isWsRun)
	{
		DataCacher cacher(5);
		SubscriberService* mysub = new SubscriberService({}, cacher);

		int i = 1;
		for (auto conf : configs)
		{
			std::cout << std::to_string(i++) + ". SERVICE RUN WITH TOPIC "
							 + conf.configs[0].topic_type_name
							 + " size: " + std::to_string(conf.configs[0].vector_size)
					  << std::endl;
			mysub->changeSubscribersConfig(conf);
			mysub->runSubscribers();
		}

		delete mysub;
	}
	else if (!isDdsRun && isWsRun)
	{
		oatpp::base::Environment::init();

		{
			Configure ws_conf;
			ws_conf.WS_HOST = conditions.ip;
			DataCacher cacher(5);
			WebsockServer server(ws_conf, cacher);

			std::thread sending([&server]() { server.run(); });

			sendingDataDto(&server, std::ref(conditions));

			sending.join();
		}

		oatpp::base::Environment::destroy();
	}
	else
	{
		auto serializeConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
		auto deserializeConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();

		auto json_object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

		PackageAnalyser* analyser = PackageAnalyser::getInstance("dto_to_str_res.txt");

		for (const auto& cond : conditions.conditions)
		{
			std::cout << formMappingTestName("Test with", cond) << std::endl;

			/*{
				std::string test_name = formMappingTestName("DTO TO WS", cond);

				analyser->addDataToAnalyse(test_name);
				MediateDataDto dto = getMediateDataDto(cond.all_vectors_sizes,
													   cond.char_vector_sizes);
				MediateDtoMapper mapper;
				for (auto i = 0; i < conditions.samples_number; ++i)
				{
					auto start = TimeConverter::GetTime_LLmcs();
					auto tmp = mapper.toWsDataDto(dto);
					auto finish = TimeConverter::GetTime_LLmcs();
					analyser->pushDataTimestamp(test_name, finish - start);
				}
				analyser->writeResults();
				analyser->clear();
			}
			{
				std::string test_name = formMappingTestName("WS TO STR", cond);
				analyser->addDataToAnalyse(test_name);
				WsDataDto::Wrapper ws_data =
					getWsDataUnion(cond.all_vectors_sizes, cond.char_vector_sizes).ws_dto;
				for (auto i = 0; i < conditions.samples_number; ++i)
				{
					auto start = TimeConverter::GetTime_LLmcs();
					auto tmp = json_object_mapper->writeToString(ws_data);
					auto finish = TimeConverter::GetTime_LLmcs();
					analyser->pushDataTimestamp(test_name, finish - start);
				}
				analyser->writeResults();
				analyser->clear();
			}

			{
				std::string test_name = formMappingTestName("DTO TO WS TO STR", cond);
				analyser->addDataToAnalyse(test_name);
				MediateDataDto dto = getMediateDataDto(cond.all_vectors_sizes,
													   cond.char_vector_sizes);
				MediateDtoMapper mapper;
				for (auto i = 0; i < conditions.samples_number; ++i)
				{
					auto start = TimeConverter::GetTime_LLmcs();
					auto tmp = mapper.toWsDataDto(dto);
					auto str = json_object_mapper->writeToString(tmp);
					auto finish = TimeConverter::GetTime_LLmcs();
					analyser->pushDataTimestamp(test_name, finish - start);
				}
				analyser->writeResults();
				analyser->clear();
			}*/

			{
				std::string test_name = formMappingTestName("DTO TO STR (CHARS AS STR)", cond);
				std::string test_name1 = formMappingTestName("STR TO OATPP STR", cond);
				analyser->addDataToAnalyse(test_name);
				MediateDataDto dto = getMediateDataDto(cond.all_vectors_sizes,
													   cond.char_vector_sizes);
				MediateDtoMapper mapper;
				for (auto i = 0; i < conditions.samples_number; ++i)
				{
					auto start = TimeConverter::GetTime_LLmcs();
					auto tmp = mapper.toString(dto);
					auto finish = TimeConverter::GetTime_LLmcs();
					analyser->pushDataTimestamp(test_name, finish - start);

					auto start1 = TimeConverter::GetTime_LLmcs();
					auto tmp1 = oatpp::String(tmp);
					auto finish1 = TimeConverter::GetTime_LLmcs();
					analyser->pushDataTimestamp(test_name1, finish1 - start1);
				}
				analyser->writeResults();
				analyser->clear();
			}

			{
				std::string test_name = formMappingTestName("DTO TO STR (CHARS AS VEC)", cond);
				std::string test_name1 = formMappingTestName("STR TO OATPP STR (CHARS AS VEC)",
															 cond);
				analyser->addDataToAnalyse(test_name);
				MediateDataDto dto = getMediateDataDto(cond.all_vectors_sizes,
													   cond.char_vector_sizes);
				MediateDtoMapper mapper;
				for (auto i = 0; i < conditions.samples_number; ++i)
				{
					auto start = TimeConverter::GetTime_LLmcs();
					auto tmp = mapper.toStringWithCharVectors(dto);
					auto finish = TimeConverter::GetTime_LLmcs();
					analyser->pushDataTimestamp(test_name, finish - start);

					auto start1 = TimeConverter::GetTime_LLmcs();
					auto tmp1 = oatpp::String(tmp);
					auto finish1 = TimeConverter::GetTime_LLmcs();
					analyser->pushDataTimestamp(test_name1, finish1 - start1);
				}
				analyser->writeResults();
				analyser->clear();
			}

			{
				std::string test_name = formMappingTestName("DTO TO OATPP STR (CHARS AS STR)",
															cond);
				analyser->addDataToAnalyse(test_name);
				MediateDataDto dto = getMediateDataDto(cond.all_vectors_sizes,
													   cond.char_vector_sizes);
				MediateDtoMapper mapper;
				for (auto i = 0; i < conditions.samples_number; ++i)
				{
					auto start = TimeConverter::GetTime_LLmcs();
					auto tmp = oatpp::String(mapper.toString(dto));
					auto finish = TimeConverter::GetTime_LLmcs();
					analyser->pushDataTimestamp(test_name, finish - start);
				}
				analyser->writeResults();
				analyser->clear();
			}

			{
				std::string test_name = formMappingTestName("DTO TO OATPP STR (CHARS AS VEC)",
															cond);
				analyser->addDataToAnalyse(test_name);
				MediateDataDto dto = getMediateDataDto(cond.all_vectors_sizes,
													   cond.char_vector_sizes);
				MediateDtoMapper mapper;
				for (auto i = 0; i < conditions.samples_number; ++i)
				{
					auto start = TimeConverter::GetTime_LLmcs();
					auto tmp = oatpp::String(mapper.toStringWithCharVectors(dto));
					auto finish = TimeConverter::GetTime_LLmcs();
					analyser->pushDataTimestamp(test_name, finish - start);
				}
				analyser->writeResults();
				analyser->clear();
			}

			/*{
				std::string test_name = formMappingTestName("DDSDATA TO DTO", cond);
				analyser->addDataToAnalyse(test_name);
				auto data = getDdsData(cond.all_vectors_sizes, cond.char_vector_sizes);
				auto additional_info = getAdditionalTopicInfo(cond.all_vectors_sizes);
				DdsDataMapper mapper;
				for (auto i = 0; i < conditions.samples_number; ++i)
				{
					auto start = TimeConverter::GetTime_LLmcs();
					auto tmp = mapper.toMediateDataDto(data, additional_info);
					auto finish = TimeConverter::GetTime_LLmcs();
					analyser->pushDataTimestamp(test_name, finish - start);
				}
				analyser->writeResults();
				analyser->clear();
			}

			{
				std::string test_name = formMappingTestName("DDSDATAEX TO DTO", cond);
				analyser->addDataToAnalyse(test_name);
				auto data = getDdsDataEx(cond.all_vectors_sizes, cond.char_vector_sizes);
				auto additional_info = getAdditionalTopicInfo(cond.all_vectors_sizes);
				DdsDataExMapper mapper;
				for (auto i = 0; i < conditions.samples_number; ++i)
				{
					auto start = TimeConverter::GetTime_LLmcs();
					auto tmp = mapper.toMediateDataDto(data, additional_info, MediateDataDto());
					auto finish = TimeConverter::GetTime_LLmcs();
					analyser->pushDataTimestamp(test_name, finish - start);
				}
				analyser->writeResults();
				analyser->clear();
			}*/
		}

		delete analyser;
	}
}