#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#if __APPLE__
	#ifdef TARGET_OS_MAC
		#include <dns_util.h>
	#endif
#elif __linux__
	#include <arpa/inet.h>
#endif
//todo check this on MacOs

//#include <arpa/inet.h>
//#include <dns_util.h>
class HttpResponse;
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Colors.hpp"


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
	std::string			_host;
	uint16_t			_port;
	int 			_state;

	char	*_buffer;
	size_t	_msgSize;

	Client();
	Client(Client const &other);

};

#endif
