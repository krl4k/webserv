// CLion
// Created by Foster Grisella on 4/13/21 15:11.
//


#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <string>
#include <iostream>
#include <istream>
#include <map>
#include "Location.hpp"
#include "Client.hpp"
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <string.h>
#include <fstream>



class Server {
public:

	Server();

	virtual ~Server();

	int createSocket();
	int getSocketFd() const;

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

	const std::string getErrorPage() const;

	void setErrorPage(const std::string &errorPage);

	const std::vector<int> &getFds() const;

	void setFds(const std::vector<int> &fds);
	void setPort(const std::string & port);
	void setPort(uint16_t port);

	const std::vector<std::string> &getWhiteList() const;

	void setWhiteList(const std::string &whiteListPath);

private:
	//todo rename this class like "VirtualHost"

	int									_socketFd;
	struct sockaddr_in					_socketAddr;

	std::string							_host;
	uint16_t							_port;
	std::string							_serverName;
	std::map<std::string, Location>		_location;
	std::string							_root;
	std::string							_errorPage;
	int 								_errorPageCode;
	std::vector<std::string>			_whiteList;
	std::string							_auth_basic_user_file;
public:
	const std::string &getAuthBasicUserFile() const;

	void setAuthBasicUserFile(const std::string &authBasicUserFile);

public:
	int getErrorPageCode() const;

private:


//	UNUSED
	Server(const Server &other);
	Server &operator=(const Server &other);



};


#endif //WEBSERV_SERVER_HPP
