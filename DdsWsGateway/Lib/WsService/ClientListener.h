#ifndef ClientListener_hpp
#define ClientListener_hpp

#include "Utilities/Ws/Callbacks.h"

#include "oatpp-websocket/AsyncConnectionHandler.hpp"
#include "oatpp-websocket/AsyncWebSocket.hpp"
#include "oatpp/core/async/Lock.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/network/ConnectionProvider.hpp"

namespace scada_ate::ws
{
class Group;

// ??????? ??????? ???????????? ??????????
class ClientListener : public oatpp::websocket::AsyncWebSocket::Listener
{
private:
	static constexpr const char* TAG = "Server_ClientListener";

private:
	std::shared_ptr<AsyncWebSocket> socket_;
	int64_t id_;

	std::shared_ptr<Group> group_;

	oatpp::data::stream::BufferOutputStream message_buffer_;
	oatpp::async::Lock write_lock_;

private:
	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, async_executor_, "server_executor");

public:
	ClientListener(const std::shared_ptr<AsyncWebSocket>& socket,
				   int64_t id,
				   std::shared_ptr<Group> group);

	int64_t getClientId() const;
	std::shared_ptr<Group> getGroup();

	CoroutineStarter onPing(const std::shared_ptr<AsyncWebSocket>& socket,
							const oatpp::String& message) override;
	CoroutineStarter onPong(const std::shared_ptr<AsyncWebSocket>& socket,
							const oatpp::String& message) override;
	CoroutineStarter onClose(const std::shared_ptr<AsyncWebSocket>& socket,
							 v_uint16 code,
							 const oatpp::String& message) override;
	CoroutineStarter readMessage(const std::shared_ptr<AsyncWebSocket>& socket,
								 v_uint8 opcode,
								 p_char8 data,
								 oatpp::v_io_size size) override;

	void sendMessageAsync(const oatpp::String& message);
	void sendCloseAsync();

	void invalidateSocket();

	void sendMessageAsync(const oatpp::String& message,
						  const BeforeMessageSend& before_message_send);
};
} // namespace scada_ate::ws

#endif // !ClientListener_hpp
