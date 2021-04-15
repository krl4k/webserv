//
// Created by Magnemite Winter on 4/13/21.
//
#include "CGI.hpp"
#include <map>
#include <iostream>

int main() {
	Client *client = new Client();
	CGI *cgi = new CGI(client, std::string("some Path"));
	std::map<std::string, std::string> env = cgi->getEnvironment();
	for (std::map<std::string,std::string>::iterator it = env.begin(); it != env.end(); it++)
		std::cout << "key: " << it->first << ", value: " << it->second << std::endl;
}
