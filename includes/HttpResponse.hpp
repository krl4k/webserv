// CLion
// Created by Foster Grisella on 4/15/21 10:01.
//


#ifndef WEBSERV_HTTPRESPONSE_HPP
#define WEBSERV_HTTPRESPONSE_HPP

#include <string>
#include <iostream>


class HttpResponse {
public:

	HttpResponse();

	virtual ~HttpResponse();


private:
	char	*_buffer;
	int		startMsg;
	size_t	_bufferSize;

	HttpResponse(const HttpResponse &other);

	HttpResponse &operator=(const HttpResponse &other);


};


#endif //WEBSERV_HTTPRESPONSE_HPP
