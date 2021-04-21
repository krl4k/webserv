// CLion
// Created by Foster Grisella on 4/14/21 12:54.
//


#include "../includes/HttpRequest.hpp"

HttpRequest::HttpRequest() : _sBuffer(),
							 _state(State::NEED_INFO), _parserState(ParserState::QUERY_STRING),
							 _method(), _path(), _queryString(), _headers(), _body(), _chunk(), _bodyStart(0),
							 _chunkPoint(0) {
	it = 0;
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
	_bodyStart = 0;
	_chunkPoint = 0;
	for (size_t i = 0; i < _chunk.size(); ++i)
		delete _chunk[i];
	_chunk.clear();

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
	if (_parserState == ParserState::QUERY_STRING)
		queryStringParse();
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
	std::cout << "path  = " << _path << std::endl;
	std::cout << "query string  = " << _queryString << std::endl;
	std::cout << "------------HEADERS------------" << std::endl;
	std::cout << "HEADERS COUNT = " << _headers.size() << std::endl;
	for (auto mapIterator = _headers.begin(); mapIterator != _headers.end(); ++mapIterator) {
		std::cout << mapIterator->first << ": " << mapIterator->second << std::endl;
	}
	std::cout << "-------------------------------" << std::endl;
	std::cout << "body:\n" << _body << std::endl;
	std::cout << MAGENTA << "----------------------------" << std::endl;
#endif

}

void HttpRequest::queryStringParse() {
	if (_sBuffer.find(CRLF) != std::string::npos) {
		_method = std::string(_sBuffer, 0, _sBuffer.find(" "));
		_path = std::string(_sBuffer, _method.length() + 1,
							_sBuffer.find(" ", _method.length() + 1, 1) - _method.length());

		size_t queryStringPos = 0;
		if ((queryStringPos = _path.find('?')) != std::string::npos) {
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
		if (_method == "POST" or _method == "PUT") {
			_parserState = ParserState::BODY;
			_bodyStart = _headersEndPos;
		} else
			_parserState = ParserState::FINISHED;
	}
}

void HttpRequest::bodyParse() {
	// todo read content lenght

	/*
	if (_headers["Transfer-Encoding"] == _headers.end()->second) {

	}
	*/

	//_headers["Transfer-Encoding"] != _headers.end()->second and
	if (_headers["TRANSFER-ENCODING"] == "chunked") {
		// todo do some magic with chunk`s
		std::cout << CYAN << "CHUNKED!!!" << RESET << std::endl;
		chunkParse();
	} else {
		size_t contentLength = 0;
		try {
			contentLength = std::stoi(_headers["CONTENT-LENGTH"]);
		} catch (std::exception &exception) {
			std::cerr << exception.what() << std::endl;
			_parserState = ParserState::FINISHED;
			return;
		}
		size_t bodyStart = _sBuffer.find(BODY_SEP) + 4;
		// todo check >= and set string(bodystart, contentLength)!
		if (_sBuffer.size() - bodyStart == contentLength) {
			_body = std::string(_sBuffer, bodyStart);
		} else {
			//wait for the end of the request
			return;
		}
		_parserState = ParserState::FINISHED;
	}
//	_parserState = ParserState::FINISHED;
}


std::pair<std::string, std::string> HttpRequest::getPair(const std::string &line) {
	std::string key = std::string(line, 0, line.find(" ") - 1);
	std::string value = std::string(line, key.size() + 2, line.find(" "));
	for (int i = 0; i < key.size(); ++i) {
		key[i] = static_cast<char>(toupper(key[i]));
	}
	return std::pair<std::string, std::string>(key, value);
}

int hexToInt(const std::string &sizeChunk) {
	unsigned int x;
	try {
		x = std::stoul(sizeChunk, nullptr, 16);
	} catch (std::exception &exception) {
		std::cout << exception.what() << std::endl;
	}
	return (x);
}

void HttpRequest::chunkParse() {
	if (_chunk.empty() or _chunk.back()->isBufferFull()) {
		_chunk.push_back(new ChunkedRequest());
	}
	std::string chunkBuffer(_sBuffer, _bodyStart + _chunkPoint);
	std::cout << "chunk size = " << chunkBuffer.size() << std::endl;

//	std::string sizeChunk(_sBuffer, _bodyStart, _sBuffer.find(CRLF, _bodyStart));
	std::cout << "chunk:\n" << chunkBuffer;
	std::cout << "----------------------------" << std::endl;

	for (size_t i = 0; i < chunkBuffer.size();) {
		size_t crlfPos = 0;
		std::string str = std::string(chunkBuffer, i);
		if (_chunk.back()->isBufferFull() and _chunk.back()->isSizeFull()) {
			_chunk.push_back(new ChunkedRequest());
		}
		// read size chunk
		if ((crlfPos = chunkBuffer.find(CRLF, i)) != std::string::npos) {
			if (!_chunk.back()->isSizeFull()) {
				_chunk.back()->setSize(std::string(chunkBuffer, i, crlfPos - i));
				try {
					_chunk.back()->setSizeFull(true);
				} catch (std::exception &exception) {
					std::cerr << "Chunk size error!!!" << std::endl;
					//check
				}
				if (_chunk.back()->getIntSize() == 0) {
					std::cout << MAGENTA << "finish" << RESET << std::endl;
					_parserState = ParserState::FINISHED;
					return;
				}
				i += crlfPos + 2 - i;
				str = std::string(chunkBuffer, i);
			}
		}
		// read buffer chunk
		if ((crlfPos = chunkBuffer.find(CRLF, i)) != std::string::npos) {
			if (!_chunk.back()->isBufferFull()) {
				_chunk.back()->setBuffer(std::string(chunkBuffer, i, _chunk.back()->getIntSize()));
				_chunk.back()->setBufferFull(true);
				i += _chunk.back()->getBuffer().size() + 2;
				str = std::string(chunkBuffer, i);
			}
		}
		if (!_chunk.back()->isSizeFull()) {
			_chunk.back()->setSize(std::string(chunkBuffer, i));
			break;
		}
		if (!_chunk.back()->isBufferFull()) {
			_chunk.back()->setBuffer(std::string(chunkBuffer, i));
			break;
		}

	}


}


//POST / HTTP/1.1
//Host: localhost:8000
//Connection: keep-alive
//		Accept-Encoding: gzip, deflate
//		Accept: */*
//User-Agent: python-requests/2.25.1
//Transfer-Encoding: chunked
//
//0xa
//abababababababababab
