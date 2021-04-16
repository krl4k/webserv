// CLion
// Created by Foster Grisella on 4/14/21 12:54.
//


#ifndef WEBSERV_HTTPREQUEST_HPP
#define WEBSERV_HTTPREQUEST_HPP

#include <string>
#include <iostream>


class HttpRequest {
public:

	enum State{
		FULL,
		NEED_INFO,
	};

	HttpRequest();

	HttpRequest(char *buffer, size_t bufferLen);

	void parse(char *buffer, int bufSize);

	virtual ~HttpRequest();

	HttpRequest(const HttpRequest &other);

	HttpRequest &operator=(const HttpRequest &other);

	void setState(int state);

	char *getBuffer() const;

	int getState() const;

	void clean(){
//		free(_buffer);
//		_buffer = strdup("");
//		_bufferLen = 0;
	}


private:
	char	*_buffer;
	size_t	_bufferLen;
	bool 	_full;
	int		_state;
};

#endif //WEBSERV_HTTPREQUEST_HPP
