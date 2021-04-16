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

	void generate();

	void clean(){
		//free(_buffer);
		this->_bufferSize = 0;
		this->_buffer = strdup("");
	}

private:
	char	*_buffer;
	int		startMsg;
	size_t	_bufferSize;

	HttpResponse(const HttpResponse &other);

	HttpResponse &operator=(const HttpResponse &other);


};


#endif //WEBSERV_HTTPRESPONSE_HPP
