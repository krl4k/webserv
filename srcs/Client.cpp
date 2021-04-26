//
// Created by Magnemite Winter on 4/11/21.
//


#include "../includes/Client.hpp"


Client::Client(int fd, const std::string &host, uint16_t port, const sockaddr_in &addr) :
	_socketFd(fd),_addr(addr), _port(port),  _host(host){
	_request = new HttpRequest();
	_response = new HttpResponse();
	_state = Client__State__REQUEST_PARSE;
	_clientInfo = new char[100];
	socklen_t addrSize = sizeof(sockaddr_in);
	inet_ntop(AF_INET, &_addr, _clientInfo, addrSize);
}

Client::~Client() {
	std::cout << RED << "Close connection, FD = " << _socketFd << RESET << std::endl;
	close(_socketFd);
	delete _request;
	delete _clientInfo;

//	delete _response;
}



Client::Client() {}

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

char *Client::getInfo() const {
	return _clientInfo;
}

const std::string &Client::getHost() const
{
	return _host;
}

void Client::setHost(const std::string &host)
{
	_host = host;
}

uint16_t Client::getPort() const
{
	return _port;
}

void Client::setPort(uint16_t port)
{
	_port = port;
}
