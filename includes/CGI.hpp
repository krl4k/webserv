 //Created on 13.04.20 by
//Roman E. aka Magnemite Winter

#ifndef WEBSERV_CGI_H
#define WEBSERV_CGI_H

#include <sys/stat.h>
class HttpResponse;
class HttpRequest;
class Client ;
#include "../includes/Client.hpp"
#include "../includes/HttpRequest.hpp"
#include "../includes/HttpResponse.hpp"
#include "../includes/Server.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <ios>
#include "Colors.hpp"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

class CGI {
public:

	CGI(Server *server, Client *client, const char *path);
	virtual ~CGI();

	void 	setArguments();
	char	**getEnvironment() const;
	char	**setEnvToString(std::map<std::string, std::string> env) ;
	void	executeCGI();

private:
	int IN = 0;
	int OUT = 1;
	CGI &operator=(const CGI &other);
	char		**_environment;
	char			**_arguments;
	char			*_path;
	HttpResponse	*_response;
	HttpRequest		*_request;
	long 			_bodySize;
	CGI(const CGI &other){};

	void 			setEnvironment(Server *server, Client *client);
	char **clone(char **other);
};


#endif //WEBSERV_CGI_H
