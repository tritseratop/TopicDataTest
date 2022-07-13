#include "PublisherProject/Config.h"

namespace scada_ate::dds::publisher
{
bool operator==(const Config& lhs, const Config& rhs)
{
	return lhs.pub_id == rhs.pub_id && lhs.vector_size == rhs.vector_size
		   && lhs.topic_name == rhs.topic_name && lhs.topic_type_name == rhs.topic_type_name
		   && lhs.topic_type == rhs.topic_type;
}
} // namespace scada_ate::dds::publisher