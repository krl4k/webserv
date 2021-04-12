#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP
#include "TCPServer.hpp"
#include <string>
#include <iostream>

class Client {

private:
	int _socket;
	struct sockaddr_in _address;
	std::string *_buffer;

public:
	Client(int socket, struct sockaddr_in address);
	Client(Client const &other);

	uint32_t getHost() const;
	uint16_t getPort() const;

	int loadData();
	std::string *getData();
	bool sendData(const char * buffer, const size_t size) const;
};


#endif
