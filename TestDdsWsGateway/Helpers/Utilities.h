#ifndef UTILITIES_H_
#define UTILITIES_H_

#include "DdsWsGatewayService/Utilities/WsCommonClasses.h"

void runWsConnection(TestCallback& test_callback, OnMessageRead& on_message_read);

class Cacher;
void runWsConnection(TestCallback& test_callback,
					 OnMessageRead& on_message_read,
					 std::shared_ptr<Cacher> cacher);

#endif //!UTILITIES_H_