// CLion
// Created by Foster Grisella on 4/20/21 14:44.
//


#include "../includes/ChunkedRequest.hpp"

ChunkedRequest::ChunkedRequest() : _bufferFull(false) , _sizeFull(false){

}

ChunkedRequest::~ChunkedRequest() {
}


ChunkedRequest::ChunkedRequest(const ChunkedRequest &other) {
	*this = other;
}

ChunkedRequest &ChunkedRequest::operator=(const ChunkedRequest &other) {
	if (this == &other)
		return *this;
	return *this;
}

const std::string &ChunkedRequest::getBuffer() const {
	return _buffer;
}


bool ChunkedRequest::isBufferFull() const {
	return _bufferFull;
}

bool ChunkedRequest::isSizeFull() const {
	return _sizeFull;
}

void ChunkedRequest::setBuffer(const std::string &buffer) {
	_buffer.append(buffer);
}

void ChunkedRequest::setSize(const std::string &size) {
	_size.append(size);
}

void ChunkedRequest::setBufferFull(bool bufferFull) {
	_bufferFull = bufferFull;
}

void ChunkedRequest::setSizeFull(bool sizeFull) {
	_sizeFull = sizeFull;
	if (_sizeFull == true) {
		try {
			_intSize = std::stoul(_size, nullptr, 16);
		}catch (std::exception &exception){
			std::cerr << exception.what() << std::endl;
			std::cerr << "str = " << _size << std::endl;
		}
	}
}

int ChunkedRequest::getIntSize() const {
	return _intSize;
}


