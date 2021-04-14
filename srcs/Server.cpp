// CLion
// Created by Foster Grisella on 4/13/21 15:11.
//


#include "../includes/Server.hpp"

Server::Server(){
}

int Server::getSocketFd() const {
	return _socketFd;
}

void Server::setSocketFd(int socketFd) {
	_socketFd = socketFd;
}

const sockaddr_in &Server::getSocketAddr() const {
	return _socketAddr;
}

void Server::setSocketAddr(const sockaddr_in &socketAddr) {
	_socketAddr = socketAddr;
}

const std::string &Server::getHost() const {
	return _host;
}

void Server::setHost(const std::string &host) {
	_host = host;
}

const std::string &Server::getPort() const {
	return _port;
}

void Server::setPort(const std::string &port) {
	_port = port;
}

const std::string &Server::getServerName() const {
	return _serverName;
}

void Server::setServerName(const std::string &serverName) {
	_serverName = serverName;
}

const std::map<std::string, Location> &Server::getLocation() const {
	return _location;
}

void Server::setLocation(const std::map<std::string, Location> &location) {
	_location = location;
}

const std::string &Server::getRoot() const {
	return _root;
}

void Server::setRoot(const std::string &root) {
	_root = root;
}

const std::pair<std::string, std::string> &Server::getErrorPage() const {
	return _errorPage;
}

void Server::setErrorPage(const std::pair<std::string, std::string> &errorPage) {
	_errorPage = errorPage;
}

int Server::createSocket() {
	struct addrinfo hints;
	struct addrinfo *servInfo;

	hints.ai_family = 0;
//	std::cout << "sizeof hints " << sizeof(hints) << std::endl;

//	memset(servInfo, 0, sizeof(hints));
//	bzero(&hints, sizeof(addrinfo));

//	memset(&hints, 0,1);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((_socketFd = socket(PF_INET, hints.ai_socktype, 0)) == -1){
		std::cerr << "socket descriptor failure" << std::endl;
		return -1;
	}

	int yes = 1;
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
		std::cerr << "set socket opt error" << std::endl;
		close(_socketFd);
		return -2;
	}



	struct addrinfo *temp;
	struct sockaddr_in *sockaddrIn = (sockaddr_in *)malloc(sizeof(sockaddr_in));

	bzero(&sockaddrIn, sizeof sockaddrIn);
	sockaddrIn->sin_family = AF_INET;
//	sockaddrIn->sin_addr.s_addr = inet_addr(_host.c_str());
	sockaddrIn->sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddrIn->sin_port = htons(8000);

	if (bind(_socketFd, (struct sockaddr *)(&sockaddrIn), sizeof(sockaddrIn)) == -1){
		std::cerr << "bind failure!" << std::endl;
		return -3;
	}
	if (listen(_socketFd, 1000) == -1){
		std::cerr << "listen failure!" << std::endl;
		return -4;
	}
	return _socketFd;
}

const std::vector<int> &Server::getFds() const {
	return _fds;
}

void Server::setFds(const std::vector<int> &fds) {
	_fds = fds;
}

Server::~Server() {
}

//void server::addrInfoInit() {
//	ft_memset(&_hints, 0, sizeof (_hints));
//	_hints.ai_family = PF_INET;
//	_hints.ai_socktype = SOCK_STREAM;
//	_hints.ai_flags = AI_PASSIVE;
//}
//
//void server::socketInit() {
//	addrInfoInit();
//	int temp = 1;
//	if ((_socket = socket(_hints.ai_family, _hints.ai_socktype, 0)) == -1){
//		std::cout << "socket descriptor failure" << std::endl;
//
//	}
//	/** setsockopt sets the options specified by the arguments.
//	 *  If we want to use our socket again, we need to specify that
//	 *  we can reuse it.
//	 * */
//	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(int)) == -1){
//		std::cout << "set socket opt error" << std::endl;
//		close(_socket);
//		throw std::exception();
//	}
//}
//
//void server::bind_socket() {
//	struct addrinfo *temp;
//	int result;
//
//	std::string IP = "127.0.0.1";
//	/** https://www.opennet.ru/docs/RUS/socket/node4.html */
//
//	_sockaddr->sin_family = AF_INET;
//	_sockaddr->sin_port = htons(_port); /** put port here*/
//	_sockaddr->sin_addr.s_addr = inet_addr(IP);/** PUT IP HERE or htonl*/
//
//	//_hints.ai_addr = _sockaddr;
//
//	if (bind(_socket, (stdruct sockaddr *)(&_sockaddr), sizeof (_sockaddr)) == -1){
//		std::cerr << "Failed to bind" << std::endl;
//		throw std::exception();
//	}
//	connectServ();
//}
//
//void server::connectServ() {
//	int backlog = 100;
//	if (listen(_socket, backlog) == -1){
//		std::cerr << "Failed to listen" << std::endl;
//	}
//}
