#include "WsClient/WSListener.hpp"
#include "WsClient/WSClient.hpp"

#include "oatpp/parser/json/Beautifier.hpp"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WSListener

void WSListener::setLogin(std::string login_)
{
	login = login_;
}

std::string WSListener::getLogin()
{
	return login;
}

oatpp::async::CoroutineStarter WSListener::onPing(const std::shared_ptr<AsyncWebSocket>& socket,
												  const oatpp::String& message)
{
	OATPP_LOGD(TAG, "onPing");
	//std::lock_guard<std::mutex> lock(m_writeMutex);
	return socket->sendPongAsync(message);
}

oatpp::async::CoroutineStarter WSListener::onPong(const std::shared_ptr<AsyncWebSocket>& socket,
												  const oatpp::String& message)
{
	OATPP_LOGD(TAG, "onPong");
	return nullptr;
}

oatpp::async::CoroutineStarter WSListener::onClose(const std::shared_ptr<AsyncWebSocket>& socket,
												   v_uint16 code,
												   const oatpp::String& message)
{
	OATPP_LOGD(TAG, "onClose code=%d", code);
	return nullptr;
}

oatpp::async::CoroutineStarter
WSListener::readMessage(const std::shared_ptr<AsyncWebSocket>& socket,
						v_uint8 opcode,
						p_char8 data,
						oatpp::v_io_size size)
{
	if (size == 0)
	{ // message transfer finished
		on_message_read_(message_buffer_.toString());

		message_buffer_.setCurrentPosition(0);
	}
	else if (size > 0)
	{ // message frame received
		message_buffer_.writeSimple(data, size);
	}
	return nullptr;
}

std::shared_ptr<oatpp::websocket::AsyncWebSocket> ClientCoroutine::SOCKET(nullptr);

oatpp::async::Action ClientCoroutine::act()
{
	return m_connector->connectAsync("ws/test/0").callbackTo(&ClientCoroutine::onConnected);
}

oatpp::async::Action ClientCoroutine::onConnected(
	const oatpp::provider::ResourceHandle<oatpp::data::stream::IOStream>& connection)
{
	m_socket = oatpp::websocket::AsyncWebSocket::createShared(connection, true);
	//m_socket->setListener(std::make_shared<WSListener>());
	m_socket->setListener(std::make_shared<WSListener>(client, on_message_read_));
	std::cout << "Connected" << std::endl;
	SOCKET = m_socket;
	return m_socket->listenAsync().next(yieldTo(&ClientCoroutine::onFinishListen));
}

oatpp::async::Action ClientCoroutine::onFinishListen()
{
	PackageAnalyser* analyser = PackageAnalyser::getInstance();
	analyser->writeResultsAndClear("");
	client->stop();
	return finish();
}