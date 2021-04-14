// CLion
// Created by Foster Grisella on 4/14/21 12:54.
//


#ifndef WEBSERV_HTTPREQUEST_HPP
#define WEBSERV_HTTPREQUEST_HPP

#include <string>
#include <iostream>


class HttpRequest {
public:

	HttpRequest();

	HttpRequest(char *buffer, size_t bufferLen);

	virtual ~HttpRequest();

	HttpRequest(const HttpRequest &other);

	HttpRequest &operator=(const HttpRequest &other);

private:
	char	*_buffer;
	size_t	_bufferLen;
	bool 	_full;
};


#endif //WEBSERV_HTTPREQUEST_HPP
