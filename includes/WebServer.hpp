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
#include <fcntl.h>

class WebServer {
public:

	WebServer(const char *fileName, int countMaxFd = 1000);
	WebServer();

	virtual ~WebServer();

	WebServer(const WebServer &other);

	WebServer &operator=(const WebServer &other);




private:
	std::string 			_configFileName;
	std::vector<Client *> 	_client;
	std::vector<Server *> 	_server;
	uint					_countMaxFd;

	int test(std::vector<Server *> vector);
	int lifeCycle();


	int testCycle();

	void handle_connection(int clientSocket);

	int acceptNewConnection();
};

#endif //WEBSERV_WEBSERVER_HPP
