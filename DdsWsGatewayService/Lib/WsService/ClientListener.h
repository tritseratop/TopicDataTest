#ifndef ClientListener_hpp
#define ClientListener_hpp

#include "Lib/Notifier/DataCacher.h"

#include "oatpp-websocket/AsyncConnectionHandler.hpp"
#include "oatpp-websocket/AsyncWebSocket.hpp"
#include "oatpp/core/async/Lock.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/network/ConnectionProvider.hpp"

class WsSocketListener;
class WebsockServer;

// слушает события подключенных соединений
class ClientListener : public oatpp::websocket::AsyncWebSocket::Listener
{
private:
	static constexpr const char* TAG = "Server_ClientListener";

private:
	std::shared_ptr<AsyncWebSocket> socket_;
	WsSocketListener* sock_listener_;
	v_int64 client_id_;
	oatpp::data::stream::BufferOutputStream message_buffer_;
	oatpp::async::Lock write_lock_;

private:
	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, async_executor_, "server_executor");
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
					server_connection_provider_,
					"server_connection_provider");

public:
	ClientListener(const std::shared_ptr<AsyncWebSocket>& socket,
				   WsSocketListener* sock_listener,
				   DataCacher& cache_,
				   v_int64 id)
		: socket_(socket)
		, sock_listener_(sock_listener)
		, client_id_(id)
	{ }
	v_int64 getClientId();
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
	void runTestMessageSendingAsync(WebsockServer* server,
									int64_t sleep,
									int64_t initial_disp,
									size_t times);

	void runDataSendingAsync(WebsockServer* server, int64_t sleep);
	void sendCloseAsync();
	void invalidateSocket();
};

#endif // !ClientListener_hpp
