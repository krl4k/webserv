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

#define ParserState__QUERY_STRING 	0
#define ParserState__HEADERS 		1
#define ParserState__BODY 			2
#define ParserState__FINISHED		3

#define HttpRequest__State__FULL					0
#define HttpRequest__State__NEED_INFO				1

class HttpRequest {
public:

	HttpRequest();

	void parse(char *buffer, ssize_t bufSize);

	virtual ~HttpRequest();

	char *getBuffer() const;

	int getState() const;

	void setState(int state);

	void clean();

	std::string getContentType() const;

	int getParserState() const;

	const std::string &getMethod() const;

	const std::string &getPath() const;

	const std::string &getQueryString() const;

	const std::map<std::string, std::string> &getHeaders() const;

	const std::string &getBody() const;
	void setContentType(std::string);
	std::string getConnectionType();

private:
	std::string _sBuffer;
	int _state;
	int _parserState;
	std::string _method;
	std::string _path;
	std::string _queryString;
	std::map<std::string, std::string> _headers;
	std::string _body;
	size_t 		_bodyStart;
	size_t		_chunkPoint;
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
