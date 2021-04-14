// CLion
// Created by Foster Grisella on 4/13/21 15:01.
//


#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP

#include <string>
#include <iostream>
#include <vector>
#include "Client.hpp"
#include "Server.hpp"
#include "Parser.hpp"

class WebServer {
public:
	WebServer(const char *fileName, int countMaxFd = 1000);
	WebServer();

	virtual ~WebServer();

	WebServer(const WebServer &other);

	WebServer &operator=(const WebServer &other);
	int lifeCycle();




		private:
	std::string 			_configFileName;
	std::vector<Client *> 	_client;
	std::vector<Server *> 	_server;
	uint					_countMaxFd;
};

#endif //WEBSERV_WEBSERVER_HPP
