#include "PublisherProject/Service.h"

#include "DdsWsGatewayService/Utilities/Dds/TestUtility.h"

#include <fstream>

using namespace scada_ate::dds;
using namespace scada_ate::dds::publisher;

int main(int argc, char** argv)
{
	std::ifstream ifile("test.json");
	nlohmann::json json;
	ifile >> json;
	GlobalTestConditions conditions = parseJsonToGlobalTestConditions(json);

	// изменяемые настройки
	Transport transport = Transport::TCP;
	std::string ip = conditions.ip;
	bool isWsServerRun = conditions.isWsServerRun;
	bool isSync = conditions.isSync;

	ParticipantConfigure<Configure> default_service_config;
	Configure ddsdata_config = {
		0, 10, 10, "DDSData", "DDSData", TopicType::DDS_DATA, 0, 1000, isSync};
	Configure ddsdataex_config = {
		0, 10, 10, "DDSDataEx", "DDSDataEx", TopicType::DDS_DATA_EX, 0, 1000, isSync};

	std::vector<ParticipantConfigure<Configure>> configs;

	for (const auto& c : conditions.conditions)
	{
		ddsdata_config.samples = conditions.samples_number;
		ddsdata_config.sleep = c.publication_interval;
		ddsdata_config.vector_size = c.all_vectors_sizes;
		ddsdata_config.char_vector_size = c.char_vector_sizes;
		default_service_config.configs = {ddsdata_config};
		configs.push_back(default_service_config);

		ddsdataex_config.samples = conditions.samples_number;
		ddsdataex_config.sleep = c.publication_interval;
		ddsdataex_config.vector_size = c.all_vectors_sizes;
		ddsdataex_config.char_vector_size = c.char_vector_sizes;
		default_service_config.configs = {ddsdataex_config};
		configs.push_back(default_service_config);
	}

	publisher::Service* mypub = new publisher::Service();

	int i = 1;
	for (auto conf : configs)
	{
		std::cout << "\n"
				  << i++ << ". SERVICE RUN WITH TOPIC " << conf.configs[0].topic_name
				  << " size: " << conf.configs[0].vector_size << std::endl;
		mypub->changeSubsConfig(conf);
		mypub->setData();
		mypub->runPublishers();
	}

	delete mypub;
	system("pause");

	return 0;
}