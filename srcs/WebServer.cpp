// CLion
// Created by Foster Grisella on 4/13/21 15:01.
//


#include "../includes/WebServer.hpp"

WebServer::WebServer(const char *fileName, int countMaxFd) : _countMaxFd(countMaxFd) {
	if (!fileName)
		_configFileName = "/configs/default.conf";
	else
		_configFileName = fileName;

	Parser *parser;
	try {
		parser = new Parser(_configFileName);
	} catch (std::exception &exception) {
		std::cout << exception.what() << std::endl;
		throw std::exception();
	}
	_server = parser->getServers();

//	std::cout << "server = " << _server[0]->getPort() << std::endl;
//	std::cout << "server = " << _server[0]->getHost() << std::endl;

	for (int i = 0; i < _server.size(); ++i) {
		if (_server[i]->createSocket() < 0){
			//clear vectors±!
			throw std::exception();
		}
	}
	lifeCycle();
}

WebServer::WebServer() : _configFileName("/configs/default.conf"), _countMaxFd(1000) {}

typedef enum
{
	eHTTP_UNKNOWN = 0
	,eHTTP_CONNECT
	,eHTTP_DELETE
	,eHTTP_GET
	,eHTTP_HEAD
	,eHTTP_OPTIONS
	,eHTTP_PATCH
	,eHTTP_POST
	,eHTTP_PUT
	,eHTTP_TRACE
}	eHTTPMethod;

typedef struct
{
	eHTTPMethod type;
	char        path[255];
}sHTTPHeader;

void parse_http_request(const char *apstrRequest, sHTTPHeader *apHeader)
{
	int  type_length = 0;
	char type[255]   = {0};
	int  index = 0;

	apHeader->type = eHTTP_UNKNOWN;

	sscanf(&apstrRequest[index], "%s", type);
	type_length = strlen(type);

	if(type_length == 3)
	{
		if(type[0] == 'G' && type[1] == 'E' && type[2] == 'T')
			apHeader->type = eHTTP_GET;

		index += type_length + 1;
		sscanf(&apstrRequest[index], "%s", apHeader->path);
		std::cout << "Path = " << apHeader->path << std::endl;
	}
}

void send_message(int aSock, const char *apstrMessage)
{
	char buffer[65536] = { 0 };

	strcat(buffer, "HTTP/1.1 200 OK\n\n");
	strcat(buffer, "<h1>");
	strcat(buffer, apstrMessage);
	strcat(buffer, "</h1>");

	int len = strlen(buffer);

	std::cout << "msg: " << buffer << std::endl;
	send(aSock, buffer, len, 0);
}

void send_404(int aSock)
{
	const char *buffer = "HTTP/1.1 404 \n\n";
	int len = strlen(buffer);
	send(aSock, buffer, len, 0);
}

void http_request(int aSock)
{
	const int request_buffer_size = 65536;
	char      request[request_buffer_size];

	int bytes_recvd = recv(aSock, request, request_buffer_size - 1, 0);

	if (bytes_recvd < 0)
	{
		fprintf(stderr, "error recv\n");
		return;
	}
	request[bytes_recvd] = '\0';

	printf("request:\n%s\n",request);

	sHTTPHeader req;
	parse_http_request(request, &req);

	if(req.type == eHTTP_GET)
	{
		send_message(aSock,"sensor 1: 10<br> "
						   "sensor 2: 20<br>"
						   "<a href=\"http://cppprosto.blogspot.com/2017/09/blog-post_23.html\">"
						   "external</a><br><a href=\""
						   "internal\">internal</a>"
						   "welcome to the club, body</a><br><a href=\"");
	}
	else
	{
		send_404(aSock);
	}
}


int WebServer::lifeCycle(){
	while (1){
		int fd;
		fd_set readFds, writeFds;
		int maxFd = _server[(_server.size() - 1)]->getSocketFd();

		FD_ZERO(&readFds);
		FD_ZERO(&writeFds);
		for (size_t i = 0; i < _server.size(); ++i) {
			FD_SET(_server[i]->getSocketFd(), &readFds);
		}

		for (size_t i = 0; i < _client.size(); ++i) {
			fd = _client[i]->getSocketFd();
			FD_SET(fd, &readFds);
			if (_client[i]->isEmpty()){
				FD_SET(fd, &writeFds);
			}
			if (fd > maxFd)
				maxFd = fd;
		}
		/* maybe timeout!!!!!!!!!!! */

		int res = select(maxFd + 1, &readFds, NULL, NULL, NULL);
		if (res < 1){
			if (errno != EINTR){
				std::cout << "error select!" << std::endl;
			}else{
				std::cout << "signal!" << std::endl;
			}
			continue;
		}
		if (res == 0){
			std::cout << "time out handle" << std::endl;
			continue;
		}

		socklen_t l = sizeof(_server[0]->getSocketAddr());
		int fdClient = accept(_server[0]->getSocketFd(), (struct sockaddr *)&(_server[0]->getSocketAddr()), &l);

		if (fdClient > 0)
			_client.push_back(new Client(fdClient, _server[0]->getHost(), _server[0]->getPort()));

		if (FD_ISSET(_client[0]->getSocketFd(), &readFds)) {
			http_request(_client[0]->getSocketFd());
		}

//

	/*	for (size_t i = 0; i < _server.size(); ++i) {
			if (FD_ISSET(_server[i]->getSocketFd(), &readFds)){

				socklen_t l = sizeof(_server[i]->getSocketAddr());
				int fdClient = accept(_server[i]->getSocketFd(), (struct sockaddr *)&(_server[i]->getSocketAddr()), &l);
//				fcntl()
				if (fdClient > 0)
					_client.push_back(new Client(fdClient, _server[i]->getHost(), _server[i]->getPort()));
			}
		}

		for (size_t i = 0; i < _client.size(); ++i) {
			if (FD_ISSET(_client[i]->getSocketFd(), &readFds)){
				http_request(_client[i]->getSocketFd());
			}
		}*/



		if (_countMaxFd)
			break;
	}
	return (0);
}

WebServer::~WebServer() {

}
