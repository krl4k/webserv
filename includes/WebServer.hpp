// CLion
// Created by Foster Grisella on 4/13/21 15:01.
//


#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP

#include "Defines.hpp"
#include <string>
#include <iostream>
#include <vector>
#include "Client.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include "Colors.hpp"

class WebServer {
public:
	/**
	 * @brief create Webserver
	 * @param fileName
	 */
	WebServer(const char *fileName = "/configs/default.conf");

	/**
	 * delete clients and Virtual Host
	 */
	virtual ~WebServer();


	/**
	 * starting Server
	 */
	void start();

private:
	std::string 			_configFileName;
	std::vector<Client *> 	_client;
	std::vector<Server *> 	_server;
	uint					_maxFdSize;


	/**
	 * \brief Main Webserver cycle
	 *
	 */
	[[noreturn]] int lifeCycle();

	/**
	 * @brief Create and accept new client
	 * @param serverNumber
	 * @return new connected client
	 */
	Client * acceptNewConnection(int serverNumber);

	/**
	 *
	 * @param readFdSet
	 * @param writeFdSet
	 */
	void initSocketSet(fd_set &readFdSet, fd_set &writeFdSet);

	/**
	 * @param readFdSet
	 */
	void acceptNewClient(fd_set &readFdSet);

	/**
	 *
	 * @param readFdSet
	 * @param writeFdSet
	 */
	void handler(fd_set &readFdSet, fd_set &writeFdSet);

	/**
	 *
	 * @param client
	 */
	void readRequest(Client *&client);

	/**
	 *
	 * @param client
	 */
	void generateResponse(Client *&client);

	/**
	 *
	 * @param client
	 */
	void sendResponse(Client *&client);

//	UNUSED
	WebServer();
	WebServer(const WebServer &other);
	WebServer &operator=(const WebServer &other);
	Server * findServer(Client *client);

};

#endif //WEBSERV_WEBSERVER_HPP
