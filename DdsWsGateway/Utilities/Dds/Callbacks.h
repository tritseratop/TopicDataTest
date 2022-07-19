#ifndef DDS_CALLBACKS_H_
#define DDS_CALLBACKS_H_

#include <fastdds/dds/publisher/DataWriter.hpp>

#include <functional>

namespace scada_ate::dds
{
using BeforeTopicSend = std::function<void(void*)>;

using OnTopicReceived = std::function<void(std::shared_ptr<void>)>;
} // namespace scada_ate::dds

#endif //!DDS_CALLBACKS_H_