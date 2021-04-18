// CLion
// Created by Foster Grisella on 4/14/21 12:54.
//


#ifndef WEBSERV_HTTPREQUEST_HPP
#define WEBSERV_HTTPREQUEST_HPP

#include "Defines.hpp"
#include <string>
#include <iostream>
#include <map>
#include "Colors.hpp"

class HttpRequest {
public:

	enum State{
		FULL,
		NEED_INFO,
	};

	enum ParserState{
		QUERY_STRING,
		HEADERS,
		BODY,
		FINISHED
	};

	HttpRequest();

	void parse(char *buffer, int bufSize);

	virtual ~HttpRequest();

	char	*getBuffer() const;
	int		getState() const;
	void	setState(int state);



	void clean() ;

private:
	char		*_cBuffer;
	std::string	_sBuffer;
	size_t		_bufferSize;
	int			_state;
	int 		_parserState;


	std::string _method;
	std::string _path;
	std::map<std::string, std::string> _headers;
	std::string _body;





	HttpRequest(const HttpRequest &other);

	HttpRequest &operator=(const HttpRequest &other);

	std::pair<std::string, std::string> getPair(const std::string &line);

	void headersParse();

	void queryStringParse();

	void bodyParse();
};

#endif //WEBSERV_HTTPREQUEST_HPP
