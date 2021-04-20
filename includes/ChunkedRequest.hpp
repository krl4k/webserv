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



	bool isFull() const;

	const std::string &getBuffer() const;

	int getSize() const;

private:
	std::string _buffer;
	int 		_size;
	bool 		_full;



};


#endif //WEBSERV1_CHUNKEDREQUEST_HPP
