// CLion
// Created by Foster Grisella on 4/13/21 18:10.
//


#include <fstream>
#include "../includes/Parser.hpp"

Parser::~Parser() {

}

Parser::Parser(const Parser &other) {

}

Parser &Parser::operator=(const Parser &other) {
	return *this;
}

Parser::Parser(const std::string &fileName) {
	std::ofstream file(fileName);
	if (!file.is_open()){
		throw std::runtime_error("file corrupted");
	}
	//read info, parsing
	file.close();

	Server *server = new Server;
	server->setHost("127.0.0.1");

	std::string port = "8080";
	int p;
	/*
	 * look like this
	 */
	try {
		p = std::stoi(port);
	}catch (std::exception &exception){
		throw std::runtime_error("bad port!");
	}
	server->setPort(p);
	server->setServerName("ppizda");
//	std::map<std::string, Location>()
//	server->setLocation()
	_servers.push_back(server);
}


const std::vector<Server *> &Parser::getServers() const {
	return _servers;
}
