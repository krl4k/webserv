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
#include "ChunkedRequest.hpp"
#include <vector>

class HttpRequest {
public:

	enum State {
		FULL,
		NEED_INFO,
	};

	enum ParserState {
		QUERY_STRING,
		HEADERS,
		BODY,
		FINISHED
	};

	HttpRequest();

	void parse(char *buffer, int bufSize);

	virtual ~HttpRequest();

	char *getBuffer() const;

	int getState() const;

	void setState(int state);

	void clean();

private:
	//--
	int it;
	//--

	std::string _sBuffer;
	int _state;
	int _parserState;

	std::string _method;
	std::string _path;
	std::string _queryString;
	std::map<std::string, std::string> _headers;
	std::string _body;
	int			_bodyStart;
	int			_chunkPoint;
	std::vector<ChunkedRequest *> _chunk;

	HttpRequest(const HttpRequest &other);

	HttpRequest &operator=(const HttpRequest &other);

	std::pair<std::string, std::string> getPair(const std::string &line);

	void headersParse();

	void queryStringParse();

	void bodyParse();

	void parseChunk(size_t  bodyStart);

	void parseContentWithLength(size_t  bodyStart);

	void createChunkContainer();
};

#endif //WEBSERV_HTTPREQUEST_HPP
