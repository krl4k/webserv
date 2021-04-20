// CLion
// Created by Foster Grisella on 4/20/21 14:44.
//


#include "../includes/ChunkedRequest.hpp"

ChunkedRequest::ChunkedRequest() {

}

ChunkedRequest::~ChunkedRequest() {
}

bool ChunkedRequest::isFull() const {
	return _full;
}

ChunkedRequest::ChunkedRequest(const ChunkedRequest &other) {
	*this = other;
}

ChunkedRequest &ChunkedRequest::operator=(const ChunkedRequest &other) {
	if (this == &other)
		return *this;
	_buffer = other._buffer;
	_full = other._full;
	_size = other._size;
	return *this;
}

const std::string &ChunkedRequest::getBuffer() const {
	return _buffer;
}

int ChunkedRequest::getSize() const {
	return _size;
}
