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
	_maxFdSize = _server.back()->getSocketFd();

	struct timeval timeout;
	while (true) {
		FD_ZERO(&readFdSet);
		FD_ZERO(&writeFdSet);

		// listen fd
		for (size_t i = 0; i < _server.size(); ++i)
			FD_SET(_server[i]->getSocketFd(), &readFdSet);

		std::cout << "\nWaiting for connection!" << std::endl;

		for (int i = 0; i < _client.size(); ++i) {
			//clients sockets
//			int fd = _client[i]->getSocketFd();
			FD_SET(_client[i]->getSocketFd(), &readFdSet);
			if (_client[i]->getState() != Client::State::REQUEST_PARSE){
				FD_SET(_client[i]->getSocketFd(), &writeFdSet);
			}
			if (_client[i]->getSocketFd() > _maxFdSize)
				_maxFdSize = _client[i]->getSocketFd();
		}

		// todo check 5 param (timeout)
		select(_maxFdSize + 1, &readFdSet, &writeFdSet, NULL, NULL);

		for (int i = 0; i < _server.size(); ++i) {
			if (FD_ISSET(_server[i]->getSocketFd(), &readFdSet)){
				Client *newClient = acceptNewConnection(i);
				if (newClient == nullptr) {
					std::cerr << "accept error!" << std::endl;
					return (-1); // todo delete this
				}
				std::cout << "Client connected = " << newClient->getSocketFd() << std::endl;
				_client.push_back(newClient);
				if (newClient->getSocketFd() > _maxFdSize)
					_maxFdSize = newClient->getSocketFd();
//				fcntl(newClient->getSocketFd(), F_SETFL, O_NONBLOCK);
			}
		}
		for (int i = 0; i < _client.size(); ++i) {
			int fd = _client[i]->getSocketFd();

			if (FD_ISSET(fd, &readFdSet) && _client[i]->getState() == Client::State::REQUEST_PARSE){
				readRequest(_client[i]);
				if (_client[i]->getRequest()->getState() == HttpRequest::State::FULL){
					FD_SET(fd, &writeFdSet);
				}
			}

			if (FD_ISSET(fd, &writeFdSet) && _client[i]->getState() == Client::State::CREATING_RESPONSE){
				generateResponce(_client[i]);
			}

			if (FD_ISSET(fd, &writeFdSet) && _client[i]->getState() == Client::State::ACCEPT_RESPONSE){

				sendResponce(_client[i]);
//				close(fd);

				_client[i]->setState(Client::State::REQUEST_PARSE);
			}
			if (_client[i]->getState() == Client::State::CLOSE) {
				std::cout << "close connection, fd = " << _client[i]->getSocketFd() << std::endl;

				_client[i]->getRequest()->clean(); //todo really
				_client[i]->getRequest()->setState(HttpRequest::State::NEED_INFO);
				_client[i]->setState(Client::State::REQUEST_PARSE);
				std::vector<Client *>::iterator it = _client.begin() + i;
				_client.erase(it);
			}
		}
	}
}

void WebServer::readRequest(Client *&client) {
	char buffer[65534];
	int bytes_read;
	bytes_read = recv(client->getSocketFd(), buffer, 65534, 0);
	if (bytes_read <= 0) {
		std::cerr << "recv error!" << std::endl;
		client->setState(Client::State::CLOSE);
		close(client->getSocketFd());
		return;
	}
	try {
		client->getRequest()->parse(buffer, bytes_read);
	} catch (std::exception &exception) {
		std::cout << "Connection closed" << std::endl;
		client->setState(Client::State::CLOSE);
		return;
	}

	if (client->getRequest()->getState() == HttpRequest::State::FULL){
		client->setState(Client::State::CREATING_RESPONSE);
	}

//	std::cout << "request:" << client->getRequest()->getBuffer() << std::endl;


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

	pClient->getResponse()->generate();


	pClient->setState(Client::State::ACCEPT_RESPONSE);

//	pClient->getResponse().


//	send(pClient->getSocketFd(),"fuck u\n\0", 8, 0);
}

void WebServer::sendResponce(Client *&pClient) {
	std::cout << "send!" << std::endl;
	char buffer[65000];

	strcat(buffer, "HTTP/1.1 200 OK\r\n\r\n");
	strcat(buffer, "<!DOCTYPE html>\n"
				   "<html>\n"
				   "<body>\n"
				   "\n"
				   "<h1>My First Heading</h1>\n"
				   "<p>My first paragraph.</p>\n"
				   "\n"
				   "</body>\n"
				   "</html>");

	int len = strlen(buffer);
	int s = send(pClient->getSocketFd(), buffer, len, 0);
	std::cout << "send = " << s << std::endl;

	bzero(buffer, len);
	pClient->setState(Client::State::REQUEST_PARSE);

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
