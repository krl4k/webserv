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
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <ios>
#include "Colors.hpp"

class CGI {
public:

	CGI(Client *client, const char *path);
	virtual ~CGI();

	void 	setArguments();
	char	**getEnvironment() const;
	char **setEnvToString(std::map<std::string, std::string> env) const;
	void	executeCGI(Client *client);

private:
	char 			**_environment;
	char			**_arguments;
	size_t			_environmentSize;
	char			*_path;
	HttpResponse	*_response;
	HttpRequest		*_request;
	CGI(const CGI &other);
	CGI &operator=(const CGI &other);

	void 			setEnvironment();
	char *strjoin(char *s1, char *s2);
	void makeProccess();
	char **clone(char **other);
};


#endif //WEBSERV_CGI_H
