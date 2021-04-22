// CLion
// Created by Foster Grisella on 4/14/21 12:54.
//


#include "../includes/HttpRequest.hpp"

HttpRequest::HttpRequest() : _sBuffer(),
	 _state(State::NEED_INFO), _parserState(ParserState::QUERY_STRING),
	 _method(), _path(), _queryString(), _headers(), _body() {
	it =0;
}

HttpRequest::~HttpRequest() {}

HttpRequest::HttpRequest(const HttpRequest &other) {}

HttpRequest &HttpRequest::operator=(const HttpRequest &other) {
	return *this;
}

int HttpRequest::getState() const {
	return _state;
}

void HttpRequest::setState(int state) {
	_state = state;
}

void HttpRequest::clean() {
	_sBuffer.clear();
	_method.clear();
	_path.clear();
	_queryString.clear();
	_headers.clear();
	_body.clear();
	_parserState = ParserState::QUERY_STRING;
	_state = State::NEED_INFO;
	it = 0;
}

/*
*
*/

void HttpRequest::parse(char *buffer, int bufSize) {
	it++;
#if DEBUG == 1
	std::cout << "it = " << it << std::endl;
#endif
	_sBuffer.append(buffer, bufSize);
	if (_parserState == ParserState::QUERY_STRING){
		queryStringParse();
	}
	if (_parserState == ParserState::HEADERS)
		headersParse();
	if (_parserState == ParserState::BODY)
		bodyParse();
	if (_parserState == ParserState::FINISHED)
		_state = State::FULL;
#if HTTP_REQUEST_DEBUG >= 1
	switch (_parserState) {
		case 0:
			std::cout << MAGENTA << "PARSE QUERY" << RESET << std::endl;
		case 1:
			std::cout << MAGENTA << "PARSE HEADERS" << RESET << std::endl;
		case 2:
			std::cout << MAGENTA << "PARSE BODY" << RESET << std::endl;
		case 3:
			std::cout << MAGENTA << "PARSE FINISHED" << RESET << std::endl;
	}
#endif

#if HTTP_REQUEST_DEBUG == 1
	std::cout << MAGENTA << "DEBUG INFO" << RESET << std::endl;
	std::cout << "method = " << _method << std::endl;
	std::cout << "path  = "	<< _path << std::endl;
	std::cout << "query string  = "	<< _queryString << std::endl;
	std::cout << "------------HEADERS------------" << std::endl;
	std::cout << "HEADERS COUNT = " << _headers.size() << std::endl;
	for (auto mapIterator = _headers.begin(); mapIterator != _headers.end(); ++mapIterator) {
		std::cout << mapIterator->first << ": " << mapIterator->second << std::endl;
	}
	std::cout << "-------------------------------" << std::endl;
	std::cout << "body:\n" << _body << std::endl;
	std::cout <<MAGENTA<< "----------------------------" << std::endl;
#endif

}

void HttpRequest::queryStringParse() {
	if (_sBuffer.find(CRLF) != std::string::npos) {
		_method = std::string(_sBuffer, 0, _sBuffer.find(" "));
		_path = std::string(_sBuffer, _method.length() + 1, _sBuffer.find(" ", _method.length() + 1, 1) - _method.length() - 1);

		size_t queryStringPos = 0;
		if ((queryStringPos = _path.find('?')) != std::string::npos){
			_queryString = std::string(_path, queryStringPos + 1);
			_path.resize(queryStringPos);
		}
		_parserState = ParserState::HEADERS;
	}
}

void HttpRequest::headersParse() {
	if (_sBuffer.find(BODY_SEP) != std::string::npos) {
		size_t _headersEndPos = _sBuffer.find(BODY_SEP) + 4;
		for (size_t i = _sBuffer.find(CRLF) + 2;;) {
			std::string line = std::string(_sBuffer, i, _sBuffer.find(CRLF, i) - i);
			if (i + line.size() + 2 < _headersEndPos)
				i += line.size() + 2;
			else
				break;
			_headers.insert(getPair(line));
		}
		//TODO rewrite
		if (_method == "POST" or _method == "PUT")
			_parserState = ParserState::BODY;
		else
			_parserState = ParserState::FINISHED;
	}
}

void HttpRequest::bodyParse() {
	// todo read content lenght
	if (_headers["Transfer-Encoding"] == _headers.end()->second) {}

	if (_headers["Transfer-Encoding"] != _headers.end()->second and \
		_headers["Transfer-Encoding"] == "chunked") {
		// todo do some fucking magic with chunk`s
		std::cout << CYAN << "CHUNKED!!!" << RESET << std::endl;
	}
	else {
		size_t contentLength = 0;
		try {
			contentLength = std::stoi(_headers["Content-Length"]);
		} catch (std::exception &exception) {
			std::cout << exception.what() << std::endl;
		}
		size_t bodyStart = _sBuffer.find(BODY_SEP) + 4;
		if (_sBuffer.size() - bodyStart == contentLength) {
			_body = std::string(_sBuffer, bodyStart);
		} else {
			//wait for the end of the request
			return;
		}
		_parserState = ParserState::FINISHED;
	}
	_parserState = ParserState::FINISHED;





}


std::pair<std::string, std::string> HttpRequest::getPair(const std::string &line) {
	std::string key = std::string(line, 0,line.find(" ") - 1);
	std::string value = std::string(line, key.size() + 2);
	return std::pair<std::string, std::string>(key, value);
}

std::string HttpRequest::getContentType() const {
	if (_headers.find("Content-Type") == _headers.end())
		return "text/html";
	return _headers.at("Content-Type");
}

int HttpRequest::getParserState() const
{
	return _parserState;
}

const std::string &HttpRequest::getMethod() const
{
	return _method;
}

const std::string &HttpRequest::getPath() const
{
	return _path;
}

const std::string &HttpRequest::getQueryString() const
{
	return _queryString;
}

const std::map<std::string, std::string> &HttpRequest::getHeaders() const
{
	return _headers;
}

const std::string &HttpRequest::getBody() const
{
	return _body;
}
