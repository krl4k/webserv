// CLion
// Created by Foster Grisella on 4/14/21 12:54.
//


#include "../includes/HttpRequest.hpp"

HttpRequest::HttpRequest() {

}

HttpRequest::~HttpRequest() {

}

HttpRequest::HttpRequest(const HttpRequest &other) {


}

HttpRequest &HttpRequest::operator=(const HttpRequest &other) {
	return *this;
}

HttpRequest::HttpRequest(char *buffer, size_t bufferLen) {
	_state = State::NEED_INFO;
	_buffer = buffer;
	_bufferLen = bufferLen;
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


void HttpRequest::parse(char *buffer, int bufSize) {
	if (buffer[bufSize - 1] == '\n')
		_state = State::FULL;
	buffer[bufSize] = '\0';
//	_state = HttpRequest::State::FULL;
	ft_strjoin(_buffer,buffer);
//	if (strcmp(_buffer, "\r\n\r\n")) {
//
//	}
}

int HttpRequest::getState() const {
	return _state;
}
