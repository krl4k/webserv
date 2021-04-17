// CLion
// Created by Foster Grisella on 4/13/21 15:01.
//


#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP
#define closesocket(socket)  close(socket)
#define SOCKET	int
#define SET_NONBLOCK(socket) fcntl(socket, F_SETFL, O_NONBLOCK);

#include <string>
#include <iostream>
#include <vector>
#include "Client.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include <fcntl.h>

class WebServer {
public:

	WebServer(const char *fileName = "/configs/default.conf");
	WebServer();

	virtual ~WebServer();

	WebServer(const WebServer &other);

	WebServer &operator=(const WebServer &other);

	void start();

private:
	std::string 			_configFileName;
	std::vector<Client *> 	_client;
	std::vector<Server *> 	_server;
	uint					_maxFdSize;

	int test(std::vector<Server *> vector);
	int lifeCycle();


	[[noreturn]] int testCycle();

	void handle_connection(int clientSocket);

	Client * acceptNewConnection(int i);

	void readRequest(Client *&client);

	void generateResponce(Client *&pClient);

	void sendResponce(Client *&pClient);

	void initSocketSet(fd_set &, fd_set &);

	void acceptNewClient(fd_set &);

	void requestHandler(fd_set &readFdSet, fd_set &writeFdSet);
};

#endif //WEBSERV_WEBSERVER_HPP
