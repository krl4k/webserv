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
	_buffer = buffer;
	_bufferLen = bufferLen;
}
