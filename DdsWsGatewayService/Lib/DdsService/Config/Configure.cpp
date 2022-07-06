#include "Lib/DdsService/Config/Configure.h"

namespace scada_ate::dds::subscriber
{
bool operator==(const Configure& lhs, const Configure& rhs)
{
	return lhs.subscriber_id == rhs.subscriber_id && lhs.vector_size == rhs.subscriber_id
		   && lhs.topic_name == rhs.topic_name && lhs.topic_type_name == rhs.topic_type_name
		   && lhs.topic_type == rhs.topic_type;
}
} // namespace scada_ate::dds::subscriber