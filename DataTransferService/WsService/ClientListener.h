#ifndef ClientListener_hpp
#define ClientListener_hpp

#include "oatpp-websocket/AsyncConnectionHandler.hpp"
#include "oatpp/network/ConnectionProvider.hpp"
#include "oatpp-websocket/AsyncWebSocket.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/core/async/Lock.hpp"

class WsSocketListener;

// слушает события подключенных соединений
class ClientListener : public oatpp::websocket::AsyncWebSocket::Listener {
private:
	static constexpr const char* TAG = "Server_ClientListener";
private:
	std::shared_ptr<AsyncWebSocket> socket_;
	WsSocketListener* sock_listener_;
	v_int64 client_id_;
	oatpp::data::stream::ChunkedBuffer message_buffer_;
	oatpp::async::Lock write_lock_;
private:
	OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, async_executor_);
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, server_connection_provider_);
public:
	ClientListener(const std::shared_ptr<AsyncWebSocket>& socket, 
		WsSocketListener* sock_listener,
		v_int64 id)
		: socket_(socket)
		, sock_listener_(sock_listener)
		, client_id_(id)
	{}
	v_int64 getClientId();
	CoroutineStarter onPing(const std::shared_ptr<AsyncWebSocket>& socket, const oatpp::String& message) override;
	CoroutineStarter onPong(const std::shared_ptr<AsyncWebSocket>& socket, const oatpp::String& message) override;
	CoroutineStarter onClose(const std::shared_ptr<AsyncWebSocket>& socket, v_uint16 code, const oatpp::String& message) override;
	CoroutineStarter readMessage(const std::shared_ptr<AsyncWebSocket>& socket, v_uint8 opcode, p_char8 data, oatpp::v_io_size size) override;
	void sendMessageAsync(const oatpp::String& message);
	void invalidateSocket();
};

#endif // !ClientListener_hpp
