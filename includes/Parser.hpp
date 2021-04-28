// CLion
// Created by Foster Grisella on 4/13/21 18:10.
//


#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP

#include <string>
#include <iostream>
#include <vector>
#include "Server.hpp"
#include <fstream>

class Parser {
public:
	Parser(const std::string &fileName);

	virtual ~Parser();

	const std::vector<Server *> &getServers() const;

	Server *separateServers(std::string &line, size_t i);

	size_t initLocations(std::vector<std::string> &strings, size_t count, size_t i);

	std::string readFile(std::ifstream &fd);

private:
	std::vector<Server *> _servers;

	Parser(const Parser &other);

	Parser &operator=(const Parser &other);

};


#endif //WEBSERV_PARSER_HPP
