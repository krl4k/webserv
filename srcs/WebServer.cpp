// CLion
// Created by Foster Grisella on 4/13/21 15:01.
//


#include <fstream>
#include <sstream>
#include "../includes/WebServer.hpp"

WebServer::WebServer(const char *fileName){
	if (fileName)
		_configFileName = fileName;
	else
		_configFileName = "./configs/default.conf";

	Parser *parser;
	try {
		parser = new Parser(_configFileName);
	} catch (std::exception &exception) {
		throw std::runtime_error(exception.what());
	}
	_server = parser->getServers();
	delete parser;
	std::cout << BLUE << "Server inited!" << RESET << std::endl;
}

void WebServer::start() {
	for (int i = 0; i < _server.size(); ++i) {
		if (_server[i]->createSocket() < 0){
			std::cerr  << "Socket dont creating!" << std::endl;
			throw std::exception();
		}
	}
	lifeCycle();
}


WebServer::WebServer() : _configFileName("/configs/default.conf"){}

WebServer::~WebServer() {
	// todo !
}

[[noreturn]]
int WebServer::lifeCycle() {
	fd_set readFdSet, writeFdSet;
	_maxFdSize = _server.back()->getSocketFd();

	struct timeval timeout;
	while (true) {
		timeout.tv_sec = 2;
		timeout.tv_usec = 1;
		std::cout << GREEN << "\nWaiting for connection!" << RESET << std::endl;
		initSocketSet(readFdSet, writeFdSet);

		select(_maxFdSize + 1, &readFdSet, &writeFdSet, NULL, &timeout);

		acceptNewClient(readFdSet);
		requestHandler(readFdSet, writeFdSet);
	}
	return (0);
}

void WebServer::initSocketSet(fd_set &readFdSet, fd_set &writeFdSet) {
	FD_ZERO(&readFdSet);
	FD_ZERO(&writeFdSet);
	for (size_t i = 0; i < _server.size(); ++i)
		FD_SET(_server[i]->getSocketFd(), &readFdSet);
	for (int i = 0; i < _client.size(); ++i) {
		FD_SET(_client[i]->getSocketFd(), &readFdSet);
		if (_client[i]->getState() != Client::State::REQUEST_PARSE){
			FD_SET(_client[i]->getSocketFd(), &writeFdSet);
		}
		if (_client[i]->getSocketFd() > _maxFdSize)
			_maxFdSize = _client[i]->getSocketFd();
	}
}

void WebServer::acceptNewClient(fd_set &readFdSet) {
	for (int i = 0; i < _server.size(); ++i) {
		if (FD_ISSET(_server[i]->getSocketFd(), &readFdSet)){
			Client *newClient = acceptNewConnection(i);
			if (newClient == nullptr) {
				std::cerr << "Accept error!" << std::endl;
				return;
			}
			SET_NONBLOCK(newClient->getSocketFd());
			std::cout << MAGENTA << "Client connected = " << newClient->getSocketFd() << RESET << std::endl;
			_client.push_back(newClient);
			if (newClient->getSocketFd() > _maxFdSize)
				_maxFdSize = newClient->getSocketFd();
		}
	}
}

void WebServer::requestHandler(fd_set &readFdSet, fd_set &writeFdSet) {
	for (int i = 0; i < _client.size(); ++i) {
		int fd = _client[i]->getSocketFd();
		if (FD_ISSET(fd, &readFdSet) && _client[i]->getState() == Client::State::REQUEST_PARSE){
			readRequest(_client[i]);
			if (_client[i]->getState() != Client::State::CLOSE and \
					_client[i]->getRequest()->getState() == HttpRequest::State::FULL){
				FD_SET(fd, &writeFdSet);
			}
		}

		if (FD_ISSET(fd, &writeFdSet) && _client[i]->getState() == Client::State::CREATING_RESPONSE){
			generateResponce(_client[i]);
		}

		if (FD_ISSET(fd, &writeFdSet) && _client[i]->getState() == Client::State::ACCEPT_RESPONSE){
			sendResponce(_client[i]);
			_client[i]->getRequest()->clean();
			_client[i]->setState(Client::State::REQUEST_PARSE);
//			_client[i]->setState(Client::State::FINISHED);
		}
		if (_client[i]->getState() == Client::State::CLOSE) {
			std::cout << "close connection, fd = " << _client[i]->getSocketFd() << std::endl;

//			_client[i]->getRequest()->clean(); //todo really
//			_client[i]->getRequest()->setState(HttpRequest::State::NEED_INFO);
//			_client[i]->setState(Client::State::REQUEST_PARSE);
			delete _client[i];
			std::vector<Client *>::iterator it = _client.begin() + i;
			_client.erase(it);
		}
	}
}

void WebServer::readRequest(Client *&client) {
	char buffer[BUFSIZE];
	int bytes_read;
	bytes_read = recv(client->getSocketFd(), buffer, BUFSIZE, 0);
	if (bytes_read <= 0) {
		client->setState(Client::State::CLOSE);
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
	socklen_t addrSize = sizeof(sockaddr_in);
	clientSocket = accept(_server[i]->getSocketFd(), (struct sockaddr *)&clientAddr, &addrSize);
	if (clientSocket < 0){
		return nullptr;
	}
	Client *client = new Client(clientSocket, _server[i]->getHost(), _server[i]->getPort(), clientAddr);
	std::cout << YELLOW << "Client with ip " << RED << client->getInfo() << YELLOW <<  " connected"<< RESET << std::endl;
	return client;
}

void WebServer::generateResponce(Client *&pClient) {

	pClient->getResponse()->generate();

	pClient->setState(Client::State::ACCEPT_RESPONSE);
}

void WebServer::sendResponce(Client *&pClient) {
	char *buffer;

	buffer = pClient->getResponse()->getFinalResponse();

	int len = strlen(buffer);
	send(pClient->getSocketFd(), buffer, len, 0);

	bzero(buffer, len);
	pClient->setState(Client::State::REQUEST_PARSE);
	std::cout << YELLOW << "sending a response to the user with ip " << pClient->getInfo() <<  RESET << std::endl;

	std::cout << YELLOW << "Response sent" << RESET << std::endl;
}