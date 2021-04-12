#ifndef WEBSERV_TCPSERVER_HPP
#define WEBSERV_TCPSERVER_HPP

#include <cstdint>
#include <functional>
#include <thread>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "CLient.hpp"

static constexpr uint16_t bufferSize = 4096;

class TCPServer {

private:
	uint16_t _port;
	status _status;
	handlerFucntionT _handler;
	std::thread _handlerThread;
	std::list<std::thread> _clientHandlerThread;
	std::list<std::thread::id> _clientHandlingEnd;
	int _serverSocket;
	void _handlingLoop;

public:
	TCPServer(const uint16_t port, handlerFunctionT handler);
	virtual ~TCPServer();

	void setHandler(handlerFucnctionT handler);
	uint16_t getPort() const;
	void 	setPort(uint16_t const port);
	status getStatus() const;
	status restart();
	status start();
	void joinLoop();
};


#endif //WEBSERV_TCPSERVER_H
