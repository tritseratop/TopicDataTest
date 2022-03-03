#include "ClientListener.h"
#include "WsSocketListener.h"

v_int64 ClientListener::getClientId() {
	return clientId;
}

oatpp::async::CoroutineStarter ClientListener::onPing(const std::shared_ptr<AsyncWebSocket>& socket, const oatpp::String& message) {
	OATPP_LOGI(TAG, "onPing");
	return socket->sendPongAsync(message);
}
oatpp::async::CoroutineStarter ClientListener::onPong(const std::shared_ptr<AsyncWebSocket>& socket, const oatpp::String& message) {
	OATPP_LOGI(TAG, "onPong");
	return nullptr;
}
oatpp::async::CoroutineStarter ClientListener::onClose(const std::shared_ptr<AsyncWebSocket>& socket, v_uint16 code, const oatpp::String& message) {
	OATPP_LOGD(TAG, "onClose code = %d", code);
	return nullptr;
}

oatpp::async::CoroutineStarter ClientListener::readMessage(const std::shared_ptr<AsyncWebSocket>& socket, v_uint8 opcode, p_char8 data, oatpp::v_io_size size) {
	if (size == 0) {
		auto wholeMessage = m_messageBuffer.toString();
		m_messageBuffer.clear();
		// TODO что делать с принятыми сообщениями?
		return nullptr;
	}
	else if (size > 0) {
		m_messageBuffer.writeSimple(data, size);
	}
	return nullptr;
}

void ClientListener::sendMessageAsync(const oatpp::String& message) {
	class SendMessageCoroutine : public oatpp::async::Coroutine<SendMessageCoroutine> {
	private:
		oatpp::async::Lock* m_lock;
		std::shared_ptr<AsyncWebSocket> websocket;
		oatpp::String message;
	public:
		SendMessageCoroutine(oatpp::async::Lock* lock,
							 const std::shared_ptr<AsyncWebSocket> socket, 
							 const oatpp::String& message = "Hello")
			: m_lock(lock)
			, websocket(socket)
			, message(message)
		{}

		Action act() override {
			return oatpp::async::synchronize(m_lock, websocket->sendOneFrameTextAsync(message)).next(finish());
		}
	};

	if (m_socket) {
		m_asyncExecutor->execute<SendMessageCoroutine>(&m_writeLock, m_socket, message);
	}
}

void ClientListener::invalidateSocket() {
	if (m_socket) {
		m_serverConnectionProvider->invalidate(m_socket->getConnection());
	}
	m_socket.reset();
}