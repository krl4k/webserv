// CLion
// Created by Foster Grisella on 4/13/21 15:01.
//


#include <fstream>
#include "../includes/WebServer.hpp"

WebServer::WebServer(const char *fileName) : _configFileName(fileName){
	Parser *parser;
	try {
		parser = new Parser(_configFileName);
	} catch (std::exception &exception) {
		std::cout << exception.what() << std::endl;
		throw std::exception();
	}
	_server = parser->getServers();
	delete parser;
	std::cout << "Server inited!" << std::endl;
}

void WebServer::start() {
	for (int i = 0; i < _server.size(); ++i) {
		if (_server[i]->createSocket() < 0){
			std::cerr << "Socket dont creating!" << std::endl;
			throw std::exception();
		}
	}
	testCycle();
}


WebServer::WebServer() : _configFileName("/configs/default.conf"){}

WebServer::~WebServer() {
	// todo !
}


int WebServer::testCycle() {
	fd_set readFdSet, writeFdSet;


	_maxFdSize = _server[_server.size() - 1]->getSocketFd();
	FD_ZERO(&readFdSet);
	FD_ZERO(&writeFdSet);
	
	for (size_t i = 0; i < _server.size(); ++i)
		FD_SET(_server[i]->getSocketFd(), &readFdSet);

	while (true) {

		std::cout << "Waiting for connection!" << std::endl;
		for (int i = 0; i < _client.size(); ++i) {
			//clients sockets
			int fd = _client[i]->getSocketFd();
			FD_SET(fd, &readFdSet);
			if (_client[i]->getState() != Client::State::REQUEST_PARSE)
				FD_SET(fd, &writeFdSet);
			if (fd > _maxFdSize)
				_maxFdSize = fd;
		}

		// todo check 5 param (timeout)
		int r = select(_maxFdSize + 1, &readFdSet, NULL, NULL, NULL);

		for (int i = 0; i < _server.size(); ++i) {
			if (FD_ISSET(_server[i]->getSocketFd(), &readFdSet)){
				Client *newClient = acceptNewConnection(i);
				if (newClient == nullptr) {
					std::cerr << "accept error!" << std::endl;
					return (-1);
				}
				int clientSocket = newClient->getSocketFd();
				FD_SET(clientSocket, &readFdSet);
				std::cout << "Client connected = " << clientSocket << std::endl;
				_client.push_back(newClient);
				if (clientSocket > _maxFdSize)
					_maxFdSize = clientSocket;
				fcntl(clientSocket, F_SETFL, (fcntl(clientSocket, F_GETFL, 0)) | O_NONBLOCK);
			}
		}

		for (int i = 0; i < _client.size(); ++i) {
			int fd = _client[i]->getSocketFd();
//			char buffer[BUFSIZ];
//			int bytes_read = 0;

			if (FD_ISSET(fd, &readFdSet) && _client[i]->getState() == Client::State::REQUEST_PARSE){
				readRequest(_client[i]);
			}

			if (FD_ISSET(fd, &writeFdSet) && _client[i]->getState() == Client::State::CREATING_RESPONSE){
				generateResponce(_client[i]);
//				createResponse();
			}

			if (FD_ISSET(fd, &writeFdSet) && _client[i]->getState() == Client::State::ACCEPT_RESPONSE){
				//sendResponce!

				send(fd, "sdf\0", 10, 0);
				FD_CLR(_client[i]->getSocketFd(), &writeFdSet);
			}

		}
	}
}

void WebServer::readRequest(Client *&client) {
	char *buffer = (char *)malloc((BUFSIZ + 1) * (sizeof(char)));
	if (!buffer){
		std::cerr << "Malloc error!" << std::endl;
		exit(0);
	}

	int bytes_read;
	bytes_read = recv(client->getSocketFd(), buffer, BUFSIZ, 0);
	if (bytes_read < 0) {
		std::cerr << "recv error!" << std::endl;
		client->setState(Client::State::CLOSE);
		free(buffer);
		return;
	}
	client->getRequest()->parse(buffer, bytes_read);

	if (client->getRequest()->getState() == HttpRequest::State::FULL){
		client->setState(Client::State::CREATING_RESPONSE);
	}


	std::cout << "request:" << buffer << std::endl;


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



Client *WebServer::acceptNewConnection(int i) {
	int clientSocket;
	struct sockaddr_in clientAddr;
//	char clientInfo[100];
	socklen_t addrSize = sizeof(sockaddr_in);
	clientSocket = accept(_server[i]->getSocketFd(), (struct sockaddr *)&clientAddr, &addrSize);
	if (clientSocket < 0){
		std::cerr << "accept failed" << std::endl;
		return nullptr;
	}
//	inet_ntop(AF_INET, &clientAddr, clientInfo, addrSize);
	Client *client = new Client(clientSocket, _server[i]->getHost(), _server[i]->getPort(), clientAddr);
	return client;
}

void WebServer::generateResponce(Client *&pClient) {
	std::cout << "generate responce" << std::endl;

	pClient->setState(Client::State::ACCEPT_RESPONSE);

//	pClient->getResponse().


//	send(pClient->getSocketFd(),"fuck u\n\0", 8, 0);
}





/*
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
*/