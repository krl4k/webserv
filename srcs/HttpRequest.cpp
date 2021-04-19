// CLion
// Created by Foster Grisella on 4/14/21 12:54.
//


#include "../includes/HttpRequest.hpp"

HttpRequest::HttpRequest() :
	_cBuffer(nullptr), _bufferSize(0), _state(State::NEED_INFO), _parserState(ParserState::QUERY_STRING) {
}

HttpRequest::~HttpRequest() {}

HttpRequest::HttpRequest(const HttpRequest &other) {}

HttpRequest &HttpRequest::operator=(const HttpRequest &other) {
	return *this;
}


int HttpRequest::getState() const {
	return _state;
}

char *HttpRequest::getBuffer() const {
	return _cBuffer;
}

void HttpRequest::setState(int state) {
	_state = state;
}

void HttpRequest::clean() {
	free(_cBuffer);
	_cBuffer = strdup("");
	_bufferSize = 0;
	_sBuffer.clear();
}


/*
 * free s1
 * free s2
 * return concat s1 and s2
 * s1 = new str
 */

char *ft_strjoin(char *&s1, char *&s2) {
	char *res;
	int i = 0;

	if (!s1 || !s2)
		return nullptr;
	if (!(res = (char *)malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1))))
		return nullptr;
	while (*s1)
		res[i++] = (*s1)++;
	while (*s2)
		res[i++] = (*s2)++;
	free(s1);
	free(s2);
	res[i] = '\0';
	s1 = res;
	s2 = nullptr;
	return res;
}

/*
*
*/

void HttpRequest::parse(char *buffer, int bufSize) {
	buffer[bufSize] = '\0';
	_sBuffer.append(buffer, bufSize);
	if (_parserState == ParserState::QUERY_STRING)
		queryStringParse();
	if (_parserState == ParserState::HEADERS and _sBuffer.find(BODY_SEP) != std::string::npos)
		headersParse();
	if (_parserState == ParserState::BODY)
		bodyParse();
	if (_parserState == ParserState::FINISHED)
		_state = State::FULL;
//	_state = State::FULL;

#if DEBUG == 1
	std::cout << MAGENTA << "DEBUG INFO" << RESET << std::endl;
	std::cout << "method = " << _method << std::endl;
	std::cout << "path  = "	<< _path << std::endl;
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
	if (_sBuffer.find(CRLF)) {
		_method = std::string(_sBuffer, 0, _sBuffer.find(" "));
		_path = std::string(_sBuffer, _method.length() + 1, _sBuffer.find(" ", _method.length() + 1, 1) - _method.length());
		// todo split  query string from path
		_parserState = ParserState::HEADERS;
	}
}

void HttpRequest::headersParse() {
	if (_sBuffer.find(BODY_SEP) != std::string::npos) {
		size_t _headersEndPos = _sBuffer.find(BODY_SEP) + 4;
		for (size_t i = _sBuffer.find(CRLF) + 2;;) {
			std::string line = std::string(_sBuffer, i, _sBuffer.find(CRLF, i) - i);
//		std::cout << "line - " << line << std::endl;
			if (i + line.size() + 2 < _headersEndPos)
				i += line.size() + 2;
			else
				break;
			_headers.insert(getPair(line));
		}
		if (_method == "POST" or _method == "PUT")
			_parserState = ParserState::BODY;
		else
			_parserState = ParserState::FINISHED;
	}
}

void HttpRequest::bodyParse() {
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
