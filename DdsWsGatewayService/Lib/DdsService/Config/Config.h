#ifndef DDS_SUB_CONFIG_H_
#define DDS_SUB_CONFIG_H_

#include "Utilities/Dds/Config.h"

#include <cstdint>

namespace scada_ate::dds::subscriber
{
struct Config
{
	int16_t id_map = 0;
	size_t max_vector_size = 0; //TODO отдельно для каждого вектора в топике?
	std::string topic_name = "";
	std::string topic_type_name = "";
	TopicType topic_type = TopicType::UNKNOWN;

	int64_t period = 1000;

	friend bool operator==(const Config& lhs, const Config& rhs);
};
} // namespace scada_ate::dds::subscriber

#endif //!DDS_SUB_CONFIG_H_