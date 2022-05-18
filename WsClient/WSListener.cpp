#include "WSListener.hpp"
#include "Utilities/WsTestUtility.h"
#include "WSClient.hpp"

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
	static int i = 1;
	if (first_package == false)
	{
		analyser = PackageAnalyser::getInstance();
		analyser->clear();
		first_package = true;
	}
	if (size == 0)
	{ // message transfer finished
		auto timestamp = TimeConverter::GetTime_LLmcs();
		oatpp::String wholeMessage = m_messageBuffer.toString();
		client->cache(getTimeFromJsonString(wholeMessage));
		if (prev_size != 0 && prev_size != wholeMessage->size())
		{
			i = 1;
			std::cout << "recieved" << std::endl;
			analyser->writeResults();
			analyser->clear();
		}
		prev_size = wholeMessage->size();
		int64_t time = getTimeFromJsonString(wholeMessage);
		/*auto data_dto = json_object_mapper->readFromString<WsDataDto::Wrapper>(wholeMessage);
		analyser->pushPackageTimestamp(
			{data_dto->tsrv, timestamp, timestamp - data_dto->tsrv, wholeMessage->size()});*/
		//std::cout << i++ << ". Delivery time: " << timestamp - time << std::endl;
		analyser->pushPackageTimestamp({time, timestamp, timestamp - time, wholeMessage->size()});
		analyser->pushDataTimestamp("COMPARE: " + std::to_string(prev_size), timestamp - time);

		m_messageBuffer.setCurrentPosition(0);
	}
	else if (size > 0)
	{ // message frame received
		m_messageBuffer.writeSimple(data, size);
	}
	return nullptr;
}

std::shared_ptr<oatpp::websocket::AsyncWebSocket> ClientCoroutine::SOCKET(nullptr);

oatpp::async::Action ClientCoroutine::act()
{
	return m_connector->connectAsync("ws").callbackTo(&ClientCoroutine::onConnected);
}

oatpp::async::Action ClientCoroutine::onConnected(
	const oatpp::provider::ResourceHandle<oatpp::data::stream::IOStream>& connection)
{
	m_socket = oatpp::websocket::AsyncWebSocket::createShared(connection, true);
	//m_socket->setListener(std::make_shared<WSListener>());
	m_socket->setListener(std::make_shared<WSListener>(client));
	std::cout << "Connected" << std::endl;
	SOCKET = m_socket;
	return m_socket->listenAsync().next(yieldTo(&ClientCoroutine::onFinishListen));
}

oatpp::async::Action ClientCoroutine::onFinishListen()
{
	PackageAnalyser* analyser = PackageAnalyser::getInstance();
	analyser->writeResults();
	client->stop();
	return finish();
}