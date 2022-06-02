#ifndef WS_SOCKET_LISTENER_H_
#define WS_SOCKET_LISTENER_H_

#include "Lib/WsService/AdapterUnit.h"
#include "Lib/WsService/ClientListener.h"
#include "Utilities/WsCommonClasses.h"

#include <map>
#include <unordered_map>

//template<class T>
class WsSocketListener : public oatpp::websocket::AsyncConnectionHandler::SocketInstanceListener
{
private:
	static constexpr const char* TAG = "Server_WSInstanceListener";

	std::atomic<int64_t> adapter_units_counter_;
	std::unordered_map<int64_t, std::shared_ptr<AdapterUnit>> adapter_units_;
	std::mutex adapter_units_mutex_;

public:
	WsSocketListener(std::unordered_map<int64_t, std::shared_ptr<AdapterUnit>> adapter_units);

	void sendCloseToAllAsync();

	void onAfterCreate_NonBlocking(const std::shared_ptr<ClientListener::AsyncWebSocket>& socket,
								   const std::shared_ptr<const ParameterMap>& params) override;

	int64_t getClientId();
	std::shared_ptr<AdapterUnit> getAdapterUnitPtr(oatpp::String adapter_unit_id);

	void onBeforeDestroy_NonBlocking(
		const std::shared_ptr<ClientListener::AsyncWebSocket>& socket) override;
};

#endif //!WS_SOCKET_LISTENER_H_
