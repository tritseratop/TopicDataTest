#include "Lib/DataObserver/DataObserver.h"
#include "Lib/DdsService/DdsSubscriber.h"
#include "Lib/WsService/WsServer.h"
#include "Utilities/DdsTestUtility.h"
#include "Utilities/WsTestUtility.h"

void sendingDataDto(IServer* server, const GlobalTestConditions& conditions)
{
	while (!server->isConnected())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	auto json_object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
	for (auto cond : conditions.conditions)
	{
		WsDataUnion ws_data = getWsDataUnion(cond.all_vectors_sizes, cond.char_vector_sizes);
		ws_data.ws_dto->tsrv = TimeConverter::GetTime_LLmcs();

		oatpp::String data = json_object_mapper->writeToString(ws_data.ws_dto);

		for (auto i = 0; i < conditions.samples_number; ++i)
		{
			server->sendData(data);
			std::this_thread::sleep_for(std::chrono::milliseconds(cond.publication_interval));
		}
	}
	server->sendClose();
	server->stop();
}

int main(int argc, char* argv[])
{
	std::ifstream ifile("test.json");
	nlohmann::json json;
	ifile >> json;
	GlobalTestConditions conditions = parseJsonToGlobalTestConditions(json);

	// изменяемые настройки
	Transport transport = Transport::TCP;
	std::string ip = conditions.ip;
	bool isWsRun = conditions.isWsServerRun;
	bool isDdsRun = conditions.isDdsServerRun;
	std::string log_file = "logs.txt";

	ServiceConfigForTest<SubscriberConfig> default_service_config({"Participant_sub",
																   transport,
																   ip,
																   4042,
																   {"127.0.0.1"},
																   {},
																   10000,
																   10000,
																   10000,
																   10000,
																   10000,
																   10000,
																   10000,
																   10000,
																   10000,
																   10000,
																   10000,
																   10000});
	SubscriberConfig ddsdata_config = {0, 10, "DDSData", "DDSData", TopicType::DDS_DATA, 0, 1000};
	SubscriberConfig ddsdataex_config = {
		0, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, 0, 1000};
	std::vector<ServiceConfigForTest<SubscriberConfig>> configs;

	for (const auto& c : conditions.conditions)
	{
		ddsdata_config.samples = conditions.samples_number;
		ddsdata_config.sleep = c.publication_interval;
		ddsdata_config.vector_size = c.all_vectors_sizes;
		ddsdata_config.info = getAdditionalTopicInfo(c.all_vectors_sizes);
		default_service_config.configs = {ddsdata_config};
		configs.push_back(default_service_config);

		ddsdataex_config.samples = conditions.samples_number;
		ddsdataex_config.sleep = c.publication_interval;
		ddsdataex_config.vector_size = c.all_vectors_sizes;
		ddsdataex_config.info = getAdditionalTopicInfo(c.all_vectors_sizes);
		default_service_config.configs = {ddsdataex_config};
		configs.push_back(default_service_config);
	}

	if (isDdsRun && isWsRun)
	{
		oatpp::base::Environment::init();

		Configure ws_conf;
		WebsockServer server(ws_conf);

		std::vector<IServer*> servers;
		servers.push_back(&server);
		SubscriberService* mysub = new SubscriberService(default_service_config, servers);

		std::thread dds_service_thread([&]() {
			int i = 1;
			for (auto conf : configs)
			{
				std::cout << std::to_string(i++) + ". SERVICE RUN WITH TOPIC "
								 + conf.configs[0].topic_type_name
								 + " size: " + std::to_string(conf.configs[0].vector_size)
						  << std::endl;
				mysub->changeSubscribersConfig(conf);
				std::thread ws_thread([](SubscriberService* mysub) { mysub->notifyingWsService(); },
									  mysub);
				mysub->runSubscribers();
				ws_thread.join();
				server.stop();
			}
		});

		server.run();
		std::cout << "ws_server stopped" << std::endl;

		delete mysub;

		if (dds_service_thread.joinable())
		{
			dds_service_thread.join();
		}

		oatpp::base::Environment::destroy();
	}
	else if (isDdsRun && !isWsRun)
	{
		SubscriberService* mysub = new SubscriberService(default_service_config, {});

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
			WebsockServer server(ws_conf);
			IServer* server_p = &server;

			std::thread sending([&]() { server.run(); });

			sendingDataDto(server_p, std::ref(conditions));

			sending.join();
		}

		oatpp::base::Environment::destroy();
	}
	else
	{
		auto serializeConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
		auto deserializeConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();

		auto json_object_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared(
			serializeConfig, deserializeConfig);

		PackageAnalyser* analyser = PackageAnalyser::getInstance("dto_to_str_res.txt");

		for (const auto& cond : conditions.conditions)
		{
			{
				std::string test_name = "\nDTO TO WS\n\tv = "
										+ std::to_string(cond.all_vectors_sizes)
										+ " cv = " + std::to_string(cond.char_vector_sizes);
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
				std::string test_name = "WS TO STR\n\tv = " + std::to_string(cond.all_vectors_sizes)
										+ " cv = " + std::to_string(cond.char_vector_sizes);
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
				std::string test_name = "DTO TO WS TO STR\n\tv = "
										+ std::to_string(cond.all_vectors_sizes)
										+ " cv = " + std::to_string(cond.char_vector_sizes);
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
			}

			{
				std::string test_name = "DTO TO STR (CHARS AS STR)\n\tv = "
										+ std::to_string(cond.all_vectors_sizes)
										+ " cv = " + std::to_string(cond.char_vector_sizes);
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
				}
				analyser->writeResults();
				analyser->clear();
			}

			{
				std::string test_name = "DTO TO STR (CHARS AS VEC)\n\tv = "
										+ std::to_string(cond.all_vectors_sizes)
										+ " cv = " + std::to_string(cond.char_vector_sizes);
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
				}
				analyser->writeResults();
				analyser->clear();
			}

			{
				std::string test_name = "DDSDATA TO DTO\n\tv = "
										+ std::to_string(cond.all_vectors_sizes)
										+ " cv = " + std::to_string(cond.char_vector_sizes);
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
				std::string test_name = "DDSDATAEX TO DTO\n\tv = "
										+ std::to_string(cond.all_vectors_sizes)
										+ " cv = " + std::to_string(cond.char_vector_sizes);
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
			}
		}

		delete analyser;
	}
}