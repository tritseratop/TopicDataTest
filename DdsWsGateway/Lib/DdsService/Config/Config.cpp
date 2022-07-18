#include "Lib/DdsService/Config/Config.h"

namespace scada_ate::dds::subscriber
{
bool operator==(const Config& lhs, const Config& rhs)
{
	return lhs.id_map == rhs.id_map && lhs.max_vector_size == rhs.id_map
		   && lhs.topic_name == rhs.topic_name && lhs.topic_type_name == rhs.topic_type_name
		   && lhs.topic_type == rhs.topic_type;
}
} // namespace scada_ate::dds::subscriber