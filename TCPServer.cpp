#include "TCPServer.hpp"
#include <map>
TCPServer::TCPServer(const uint16_t port, int handler): _port(port), _handler(handler) {}

TCPServer::~TCPServer() {
	if (_status == status::up)
		stop();
}

void TCPServer::setHandler(TCPServer::handlerFunctionHandlerT handler) {
	_handler = handler;
}

uint16_t TCPServer::getPort() const {
	return _port;
}

void TCPServer::setPort(const uint16_t port) {
	_port = port;
	restart();
}

TCPServer::status TCPServer::restart() {
	if (_status == status::up)
		stop();
	return start();
}

void TCPServer::joinLoop() {
	_handlerThread.join();
}

int TCPServer::Client::loadData() {
	return recv(_socker, _buffer, )
}