#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#if __APPLE__
	#include <dns_util.h>
#elif __linux__
	#include <arpa/inet.h>
#endif
class HttpResponse;
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Colors.hpp"

#define Client__State__REQUEST_PARSE 0
#define Client__State__CREATING_RESPONSE 1
#define Client__State__ACCEPT_RESPONSE 2
#define Client__State__CLOSE 3


class Client {
public:
	Client(int fd, const std::string &host, uint16_t  port, const sockaddr_in &addr);

	virtual ~Client();

	const std::string &getHost() const;

	void setHost(const std::string &host);

	uint16_t getPort() const;

	void setPort(uint16_t port);

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
	uint16_t			_port;
	std::string			_host;
	int 			_state;

	char	*_buffer;
	size_t	_msgSize;

	Client();
	Client(Client const &other);

};

#endif
