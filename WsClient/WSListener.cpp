#include "WSListener.hpp"
#include "WSClient.hpp"

#include <iostream>

#include "oatpp/parser/json/Beautifier.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WSListener

void WSListener::setLogin(std::string login_) {
    login = login_;
}

std::string WSListener::getLogin() {
    return login;
}

oatpp::async::CoroutineStarter WSListener::onPing(const std::shared_ptr<AsyncWebSocket>& socket, const oatpp::String& message) {
    OATPP_LOGD(TAG, "onPing");
    //std::lock_guard<std::mutex> lock(m_writeMutex);
    return socket->sendPongAsync(message);
}

oatpp::async::CoroutineStarter WSListener::onPong(const std::shared_ptr<AsyncWebSocket>& socket, const oatpp::String& message) {
    OATPP_LOGD(TAG, "onPong");
    return nullptr;
}

oatpp::async::CoroutineStarter WSListener::onClose(const std::shared_ptr<AsyncWebSocket>& socket, v_uint16 code, const oatpp::String& message) {
    OATPP_LOGD(TAG, "onClose code=%d", code);
    return nullptr;
}

oatpp::async::CoroutineStarter WSListener::readMessage(const std::shared_ptr<AsyncWebSocket>& socket, v_uint8 opcode, p_char8 data, oatpp::v_io_size size) {
    if (size == 0) { // message transfer finished        
        auto wholeMessage = m_messageBuffer.toString();
        m_messageBuffer.clear();
        std::cout << wholeMessage->c_str() << std::endl;
    }
    else if (size > 0) { // message frame received
        m_messageBuffer.writeSimple(data, size);
    }
    return nullptr;
}

std::shared_ptr<oatpp::websocket::AsyncWebSocket> ClientCoroutine::SOCKET(nullptr);

oatpp::async::Action ClientCoroutine::act() {
    return m_connector->connectAsync("ws").callbackTo(&ClientCoroutine::onConnected);
}

oatpp::async::Action ClientCoroutine::onConnected(const std::shared_ptr<oatpp::data::stream::IOStream>& connection) {
    m_socket = oatpp::websocket::AsyncWebSocket::createShared(connection, true);
    //m_socket->setListener(std::make_shared<WSListener>());
    m_socket->setListener(std::make_shared<WSListener>(client));
    std::cout << "Connected" << std::endl;
    SOCKET = m_socket;
    return m_socket->listenAsync().next(yieldTo(&ClientCoroutine::onFinishListen));
}

oatpp::async::Action ClientCoroutine::onFinishListen() {
    return finish();
}