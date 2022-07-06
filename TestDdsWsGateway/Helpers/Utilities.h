#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "DdsWsGatewayService/Utilities/Ws/Callbacks.h"

namespace scada_ate
{
void runWsConnection(ws::TestCallback& test_callback, ws::OnMessageRead& on_message_read);

class Cacher;
void runWsConnection(ws::TestCallback& test_callback,
					 ws::OnMessageRead& on_message_read,
					 std::shared_ptr<Cacher> cacher);
} // namespace scada_ate

#endif //!UTILITIES_H_