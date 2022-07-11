#include "Lib/DdsService/Config/Configure.h"

namespace scada_ate::dds::subscriber
{
bool operator==(const Configure& lhs, const Configure& rhs)
{
	return lhs.id_map == rhs.id_map && lhs.max_vector_size == rhs.id_map
		   && lhs.topic_name == rhs.topic_name && lhs.topic_type_name == rhs.topic_type_name
		   && lhs.topic_type == rhs.topic_type;
}
} // namespace scada_ate::dds::subscriber