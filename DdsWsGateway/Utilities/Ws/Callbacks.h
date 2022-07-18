#ifndef WS_CALLBACKS_H_
#define WS_CALLBACKS_H_

#include "oatpp/core/Types.hpp"

#include <functional>

namespace scada_ate::ws
{
class Group;

using TestCallback = std::function<void(Group&)>;
using BeforeMessageSend = std::function<void(oatpp::String& message)>;

using OnMessageRead = std::function<void(const oatpp::String& message)>;
} // namespace scada_ate::ws

#endif //!WS_CALLBACKS_H_