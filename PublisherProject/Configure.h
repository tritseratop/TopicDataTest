#ifndef DDS_PUB_CONFIGURE_H_
#define DDS_PUB_CONFIGURE_H_

#include "DdsWsGatewayService/Utilities/Dds/Configure.h"

#include <cstdint>
#include <string>

namespace scada_ate::dds::publisher
{
struct Configure
{
	int16_t pub_id = 0;
	uint16_t vector_size = 0;
	uint16_t char_vector_size = 0;
	std::string topic_name = "";
	std::string topic_type_name = "";
	TopicType topic_type = TopicType::UNKNOWN;

	// listener settings
	uint32_t samples = 10;
	uint32_t sleep = 1000;

	bool isSync = false;

	friend bool operator==(const Configure& lhs, const Configure& rhs);
};
} // namespace scada_ate::dds::publisher

#endif //!DDS_PUB_CONFIGURE_H_