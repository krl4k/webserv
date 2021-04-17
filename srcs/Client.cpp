//
// Created by Magnemite Winter on 4/11/21.
//

#include <unistd.h>
#include "../includes/Client.hpp"



Client::Client(int fd, const std::string &host, uint16_t port, const sockaddr_in &addr) :
	_socketFd(fd), _host(host), _port(port), _addr(addr), _msgSize(0), _buffer(strdup("")){
	_request = new HttpRequest();
	_response = new HttpResponse();
	_state = State::REQUEST_PARSE;
}


Client::Client() {

}

int Client::getState() const {
	return _state;
}

int Client::getSocketFd() const {
	return _socketFd;
}

void Client::setState(int state) {
	_state = state;
}

HttpRequest *Client::getRequest() const {
	return _request;
}

HttpResponse *Client::getResponse() const {
	return _response;
}

Client::~Client() {
	std::cout << "client clone connection" << std::endl;
	close(_socketFd);
	delete _request;
//	delete _response;
}
