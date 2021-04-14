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
	server->setPort("8080");
	server->setServerName("ppizda");
	_servers.push_back(server);
}


const std::vector<Server *> &Parser::getServers() const {
	return _servers;
}
