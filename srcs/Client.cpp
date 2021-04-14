//
// Created by Magnemite Winter on 4/11/21.
//

#include "../includes/Client.hpp"

int Client::getSocketFd() const {
	return _socketFd;
}

bool Client::isEmpty() const {
	return _empty;
}

Client::Client(int fd, const std::string host, const std::string port) : _socketFd(fd), _host(host), _port(port){
}
