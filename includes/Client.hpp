#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP
#include <string>
#include <iostream>
#include <dns_util.h>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"



class Client {
public:
	enum State{
		REQUEST_PARSE,
		CREATING_RESPONSE,
		ACCEPT_RESPONSE,
		CLOSE
	};

	Client(int fd, const std::string &host, uint16_t  port, const sockaddr_in &addr);

	virtual ~Client();


	char *getInfo() const;


	int getSocketFd() const;
	int getState() const;

	HttpRequest *getRequest() const;

	HttpResponse *getResponse() const;

	void setState(int state);

private:
	int				_socketFd;
	HttpRequest* 	_request;
	HttpResponse*	_response;
	struct sockaddr_in _addr;
	char				*_clientInfo;
	std::string		_host;
	uint16_t 		_port;
	int 			_state;

	char	*_buffer;
	size_t	_msgSize;

	Client();
	Client(Client const &other);

};

#endif
