#ifndef WEBSERV_WEBSERVER_HPP
#define WEBSERV_WEBSERVER_HPP

#include <sys/socket.h>
#include <sys/select.h>

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define PURPLE "\x1B[35m"
#define BLUE "\x1B[34m"
#define ENDC "\x1B[0m"

class WebServer {

private:
	int _socketFD;

public:

};


#endif //WEBSERV_WEBSERVER_H
