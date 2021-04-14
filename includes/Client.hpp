#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP
#include <string>
#include <iostream>
#include <dns_util.h>
//#include <../include/HttpRequest.hpp>
#include "HttpRequest.hpp"

class Client {

public:
	Client();
	Client(Client const &other);

	Client(int fd, const std::string host, const std::string port);

	uint32_t getHost() const;
	uint16_t getPort() const;

	int loadData();
	std::string *getData();
	bool sendData(const char * buffer, const size_t size) const;


private:
//	enum state{
//		WAITING,
//		READY,
//
//	};
	int				_socketFd;
public:
	int getSocketFd() const;

private:
	HttpRequest* 	_request;
//	HttpResponse*	_response;
	std::string		_host;
	std::string		_port;
	std::string		*_buffer;
	bool 			_empty;
public:
	bool isEmpty() const;

};

#endif
