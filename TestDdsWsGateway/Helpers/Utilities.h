#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "DdsWsGateway/Lib/DdsService/Config/Config.h"

#include "DdsWsGateway/Utilities/Dds/Config.h"
#include "DdsWsGateway/Utilities/Ws/Callbacks.h"

namespace scada_ate
{
std::unordered_map<uint32_t, std::shared_ptr<void>>
getCachers(std::vector<dds::subscriber::Config> configs);
//void runDdsService(dds::ParticipantConfig<dds::publisher::Config>,
//				   dds::ParticipantConfig<dds::subscriber::Config>);

class Cacher;
namespace ws
{
void runWsConnection(ws::TestCallback& test_callback, ws::OnMessageRead& on_message_read);

void runWsConnection(ws::TestCallback& test_callback,
					 ws::OnMessageRead& on_message_read,
					 std::shared_ptr<Cacher> cacher);
} // namespace ws
} // namespace scada_ate

#endif //!UTILITIES_H_