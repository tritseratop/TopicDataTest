#ifndef WS_SOCKET_LISTENER_H_
#define WS_SOCKET_LISTENER_H_

#include "ClientListener.h"

#include <map>
#include <queue>

class WsSocketListener : public oatpp::websocket::AsyncConnectionHandler::SocketInstanceListener
{
private:
	static constexpr const char* TAG = "Server_WSInstanceListener";

	std::map<v_int64, std::shared_ptr<ClientListener>> clients_;
	std::mutex m_write_message_;
	std::mutex m_change_clients_;

public:
	static std::atomic<v_int32> SOCKETS; // for id generation
public:
	WsSocketListener();
	void addClient(const std::shared_ptr<ClientListener>& socket);

	void removeClientById(v_int64 id);
	std::shared_ptr<ClientListener> getClientById(v_int64 id);

	void sendMessageToAllAsync(const oatpp::String& message, WebsockServer* server = nullptr);
	void sendClose();

	void onAfterCreate_NonBlocking(const std::shared_ptr<ClientListener::AsyncWebSocket>& socket,
								   const std::shared_ptr<const ParameterMap>& params) override;
	void onBeforeDestroy_NonBlocking(
		const std::shared_ptr<ClientListener::AsyncWebSocket>& socket) override;

	bool isConnected();
};

#endif //!WS_SOCKET_LISTENER_H_
