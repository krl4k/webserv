// CLion
// Created by Foster Grisella on 4/13/21 15:11.
//


#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <string>
#include <iostream>
#include <map>
#include "Location.hpp"
#include "Client.hpp"
#include <netdb.h>
#include <unistd.h>
#include <vector>

class Server {
public:

	Server();

	virtual ~Server();

	Server(const Server &other);

	Server &operator=(const Server &other);

	int createSocket();

	int getSocketFd() const;

	void setSocketFd(int socketFd);

	const sockaddr_in &getSocketAddr() const;

	void setSocketAddr(const sockaddr_in &socketAddr);

	const std::string &getHost() const;

	void setHost(const std::string &host);

	uint16_t getPort() const;

	const std::string &getServerName() const;

	void setServerName(const std::string &serverName);

	const std::map<std::string, Location> &getLocation() const;

	void setLocation(const std::map<std::string, Location> &location);

	const std::string &getRoot() const;

	void setRoot(const std::string &root);

	const std::pair<std::string, std::string> &getErrorPage() const;

	void setErrorPage(const std::pair<std::string, std::string> &errorPage);
	const std::vector<int> &getFds() const;

	void setFds(const std::vector<int> &fds);

	void setPort(uint16_t port);

private:
	int									_socketFd;
	struct sockaddr_in					_socketAddr;

//	std::vector<int>					_fds;
	std::string							_host;
	uint16_t							_port;
	std::string							_serverName;
	std::map<std::string, Location>		_location;
	std::string							_root;
	std::pair<std::string, std::string>	_errorPage;

//	std::map<>


};


#endif //WEBSERV_SERVER_HPP
