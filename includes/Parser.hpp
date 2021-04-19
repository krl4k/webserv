// CLion
// Created by Foster Grisella on 4/13/21 18:10.
//


#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP

#include <string>
#include <iostream>
#include <vector>
#include "Server.hpp"

class Parser {
public:
	Parser(const std::string &fileName);

	virtual ~Parser();

	Parser(const Parser &other);

	Parser &operator=(const Parser &other);

	const std::vector<Server *> &getServers() const;

	Server * separateServers(std::string &line, size_t i);
	int initLocations(std::vector<std::string> &strings, int count, int i);
	std::string readFile(std::ifstream &fd);

private:
	std::vector<Server *> _servers;
};


#endif //WEBSERV_PARSER_HPP
