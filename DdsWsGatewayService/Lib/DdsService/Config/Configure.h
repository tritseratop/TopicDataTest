#ifndef DDS_SUB_CONFIGURE_H_
#define DDS_SUB_CONFIGURE_H_

#include "Utilities/Dds/Configure.h"

#include <cstdint>

namespace scada_ate::dds::subscriber
{
struct Configure
{
	int16_t subscriber_id = 0;
	size_t vector_size = 0;
	std::string topic_name = "";
	std::string topic_type_name = "";
	TopicType topic_type = TopicType::UNKNOWN;

	// listener settings
	size_t samples = 10;
	int64_t sleep = 1000;

	AdditionalTopicInfo info;

	bool isCache = false;

	friend bool operator==(const Configure& lhs, const Configure& rhs);
};
} // namespace scada_ate::dds::subscriber

#endif //!DDS_SUB_CONFIGURE_H_