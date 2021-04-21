// CLion
// Created by Foster Grisella on 4/20/21 14:44.
//


#ifndef WEBSERV1_CHUNKEDREQUEST_HPP
#define WEBSERV1_CHUNKEDREQUEST_HPP

#include <string>
#include <iostream>


class ChunkedRequest {
public:
	ChunkedRequest();

	virtual ~ChunkedRequest();

	ChunkedRequest(const ChunkedRequest &other);
	ChunkedRequest &operator=(const ChunkedRequest &other);

	void setBuffer(const std::string &buffer);

	void setSize(const std::string &size);

	void setBufferFull(bool bufferFull);

	void setSizeFull(bool sizeFull);

	int getIntSize() const;

	bool isBufferFull() const;

	bool isSizeFull() const;

	const std::string &getBuffer() const;


private:
	std::string _buffer;
	bool 		_bufferFull;
	std::string _size;
	bool 		_sizeFull;

	int			_intSize;
};


#endif //WEBSERV1_CHUNKEDREQUEST_HPP
