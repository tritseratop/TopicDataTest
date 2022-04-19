#include "ClientListener.h"
#include "WsSocketListener.h"

v_int64 ClientListener::getClientId() {
	return client_id_;
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
		auto wholeMessage = message_buffer_.toString();
		message_buffer_.setCurrentPosition(0);
		// TODO что делать с принятыми сообщениями?
		return nullptr;
	}
	else if (size > 0) {
		message_buffer_.writeSimple(data, size);
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

	if (socket_) {
		async_executor_->execute<SendMessageCoroutine>(&write_lock_, socket_, message);
	}
}

void ClientListener::invalidateSocket() {
	if (socket_) {
		//server_connection_provider_->invalidate(socket_->getConnection());
	}
	socket_.reset();
}