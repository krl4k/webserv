// CLion
// Created by Foster Grisella on 4/13/21 15:01.
//


#include <fstream>
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

	std::cout << "server socket = " << _server[0]->getSocketFd() << std::endl;


	for (int i = 0; i < _server.size(); ++i) {
		if (_server[i]->createSocket() < 0){
			throw std::exception();
		}
	}

//	test(_server);

//	lifeCycle();
	std::cout << "server socket = " << _server[0]->getSocketFd() << std::endl;

	testCycle();
}

WebServer::WebServer() : _configFileName("/configs/default.conf"), _countMaxFd(1000) {}//todo delete MN



int WebServer::lifeCycle(){
/*	while (1){
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
		*//* maybe timeout!!!!!!!!!!! *//*

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
//			http_request(_client[0]->getSocketFd());
		}

//

	*//*	for (size_t i = 0; i < _server.size(); ++i) {
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
		}*//*



		if (_countMaxFd)
			break;
	}
	return (0);*/
	return (0);
}

WebServer::~WebServer() {

}


int WebServer::testCycle() {
	int clientSocket;

	while (true){
		std::cout << "Waiting for connection!" << std::endl;
		clientSocket = acceptNewConnection();
		if (clientSocket < 0){
			std::cerr << "bad client socket" << std::endl;
			return -1;
		}
		std::cout << "Client connected" << std::endl;

		handle_connection(clientSocket);

	}
}

std::string readFile(const std::string& fileName) {
	std::ifstream f(fileName);
	f.seekg(0, std::ios::end);
	size_t size = f.tellg();
	std::string s(size, ' ');
	f.seekg(0);
	f.read(&s[0], size); // по стандарту можно в C++11, по факту работает и на старых компиляторах
	return s;
}

#define BUFSIZE 1024
void WebServer::handle_connection(int clientSocket) {
	char buffer[BUFSIZE];
	size_t bytes_read;
	size_t msgSize = 0;
	std::string paths[3] = {"/srcs", "/", "includes"};

	while ((bytes_read = read(clientSocket, buffer + msgSize, sizeof(buffer) - msgSize - 1)) > 0){
		msgSize += bytes_read;
		if (msgSize > BUFSIZE - 1 || buffer[msgSize - 1 ] == '\n')
			break;
	}
	if (bytes_read < 0){
		std::cerr << "recv error !" << std::endl;
		return;
	}
	buffer[msgSize - 1] = '\0';

	char actualPath[PATH_MAX];
	std::cout << "buffer = " << buffer << std::endl;
	if (realpath(buffer, actualPath) == 0){
		std::cout << "bad path: " << actualPath << std::endl;
		send(clientSocket, "bad path!!!", 10, 0);
		close(clientSocket);
		return;
	}

	std::cout << "actual path = " << actualPath << std::endl;
	std::cout << "request:\n" << buffer << std::endl;
	int fileFd = open(actualPath, O_RDONLY);
	if (fileFd < 0) {
		std::cerr << "file not found" << std::endl;
		close(clientSocket);
		return;
	}

//	sleep(1);

	while ((bytes_read = read(fileFd, buffer, BUFSIZE)) > 0){
//		std::cout << "sending " << bytes_read << " bytes" << std::endl;
		send(clientSocket, buffer, bytes_read, 0);
	}
	close(clientSocket);
	close(fileFd);
	std::cout << "connnection close" << std::endl;
}


int WebServer::test(std::vector<Server *> vector) {
	int listenFd;
	struct sockaddr_in servaddr;

//	std::cout << "server port = " << vector[0]->getPort() << std::endl;
//	std::cout << "server host = " << vector[0]->getHost() << std::endl;

	if ((listenFd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		std::cerr << "fatal error! socket!" << std::endl;
		return -1;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
//	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_addr.s_addr = inet_addr(_server[0]->getHost().c_str());
//	int port = std::stoi(_server[0]->getPort());
	servaddr.sin_port = htons(_server[0]->getPort());

	int yes = 1;
	if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		std::cerr << "error setsockopt" << std::endl;
		close(listenFd);
		return -2;
	}

	if ((bind(listenFd, (struct sockaddr * )&servaddr, sizeof servaddr)) < 0){
		std::cerr << "bind error" << std::endl;
		return -2;
	}
	if (listen(listenFd, 1000) < 0){
		std::cerr << "listen error!" << std::endl;
		return -3;
	}


	int connectFd;
	const int MAXLINE = 1024;
	char recvLine[MAXLINE];
	char sendLine[MAXLINE];
	int n;
	for (;;) {
		struct sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof clientAddr;
		char client_info[MAXLINE];
		std::cout << "waiting connection!" << std::endl;

		connectFd = accept(listenFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
		inet_ntop(AF_INET, &clientAddr, client_info, MAXLINE);

		std::cout << "Client connection: " << client_info << std::endl;

		bzero(recvLine, MAXLINE);

		while ((n = read(connectFd, recvLine, MAXLINE -1 )) > 0){
			if (recvLine[n - 1] == '\n'){
				break;
			}
			bzero(recvLine, MAXLINE);
		}
		if (n < 0){
			std::cerr << "read error!!!" << std::endl;
			return -3;
		}

		std::cout << "request:\n" << recvLine << std::endl;
		strcpy(sendLine, "HTTP/1.1 200 OK\r\n\r\nlol pizda!!!");

		std::cout << "response:\n" << sendLine << std::endl;

		send(connectFd, sendLine, strlen(sendLine), 0);
		close(connectFd);
	}


}

int WebServer::acceptNewConnection() {
	int clientSocket;
	struct sockaddr_in clientAddr;
	char clientInfo[100];
	socklen_t addrSize = sizeof(sockaddr_in);
	std::cout << "server socket = " << _server[0]->getSocketFd() << std::endl;
	clientSocket = accept(_server[0]->getSocketFd(), (struct sockaddr *)&clientAddr, &addrSize);
	if (clientSocket < 0){
		std::cerr << "accept failed" << std::endl;
		return -1;
	}
	inet_ntop(AF_INET, &clientAddr, clientInfo, addrSize);
	return clientSocket;
}

