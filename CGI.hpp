//Created on 13.04.20 by
//Roman E. aka Magnemite Winter

#ifndef WEBSERV_CGI_H
#define WEBSERV_CGI_H
class HTTPResponse{};
class HTTPRequest{};
class Client {};
//#include "Client.hpp"
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define RED "\x1B[31m"
#define ENDC "\x1B[0m"

class CGI {
public:

	CGI(Client *client, char *path);
	virtual ~CGI();
	CGI(const CGI &other);

	CGI &operator=(const CGI &other);

	void 	setArguments();
	char **getEnvironment() const;
	void executeCGI(HTTPResponse &response);

private:
	char 			**_environment;
	char			**_arguments;
	size_t			_environmentSize;
	char			*_path;
	HTTPResponse	*_response;
	HTTPRequest		*_request;

	char *strjoin(char *s1, char *s2);
	void makeProccess();
	bool checkAlloc();
};


#endif //WEBSERV_CGI_H
