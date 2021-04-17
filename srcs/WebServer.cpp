// CLion
// Created by Foster Grisella on 4/13/21 15:01.
//


#include <fstream>
#include <sstream>
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

[[noreturn]]
int WebServer::testCycle() {
	fd_set readFdSet, writeFdSet;
	_maxFdSize = _server.back()->getSocketFd();

	while (true) {
		std::cout << "\nWaiting for connection!" << std::endl;
		initSocketSet(readFdSet, writeFdSet);
		// todo check 5 param (timeout)
		std::cout << "before select" << std::endl;
		select(_maxFdSize + 1, &readFdSet, &writeFdSet, NULL, NULL);
		std::cout << "after select" << std::endl;
//		if (res < 0)//delete this
//			return -1;
		acceptNewClient(readFdSet);

		requestHandler(readFdSet, writeFdSet);
	}
}

void WebServer::initSocketSet(fd_set &readFdSet, fd_set &writeFdSet) {
	FD_ZERO(&readFdSet);
	FD_ZERO(&writeFdSet);
	for (size_t i = 0; i < _server.size(); ++i)
		FD_SET(_server[i]->getSocketFd(), &readFdSet);
	for (int i = 0; i < _client.size(); ++i) {
		FD_SET(_client[i]->getSocketFd(), &readFdSet);
		if (_client[i]->getState() != Client::State::REQUEST_PARSE && _client[i]->getState() != Client::State::FINISHED ){
			FD_SET(_client[i]->getSocketFd(), &writeFdSet);
		}
		if (_client[i]->getState() == Client::State::FINISHED){
			_client[i]->setState(Client::State::REQUEST_PARSE);
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
				std::cerr << "accept error!" << std::endl;
				return;
			}
			fcntl(newClient->getSocketFd(), F_SETFL, O_NONBLOCK);
			std::cout << "Client connected = " << newClient->getSocketFd() << std::endl;
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
			std::cout << "Tut epta" << std::endl;
			try {
				readRequest(_client[i]);
			} catch (std::exception &exception) {
				std::cerr << exception.what() << std::endl;
				_client[i]->setState(Client::State::CLOSE);
			}
			if (_client[i]->getRequest()->getState() == HttpRequest::State::FULL){
				FD_SET(fd, &writeFdSet);
			}
		}

		if (FD_ISSET(fd, &writeFdSet) && _client[i]->getState() == Client::State::CREATING_RESPONSE){
			generateResponce(_client[i]);
		}

		if (FD_ISSET(fd, &writeFdSet) && _client[i]->getState() == Client::State::ACCEPT_RESPONSE){
			sendResponce(_client[i]);
			_client[i]->getRequest()->clean();
//			close(fd);
			_client[i]->setState(Client::State::REQUEST_PARSE);
//			_client[i]->setState(Client::State::FINISHED);
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

void WebServer::readRequest(Client *&client) {
	std::cout << "Aaaaaaa" << std::endl;
	char buffer[65534];
	int bytes_read;
	bytes_read = recv(client->getSocketFd(), buffer, 65534, 0);
	if (bytes_read <= 0) {
//		std::cerr << "Recv error!" << std::endl;
		client->setState(Client::State::CLOSE);
//		close(client->getSocketFd());
		throw std::runtime_error("Client socket close!");
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
//	char clientInfo[100];
	socklen_t addrSize = sizeof(sockaddr_in);
	clientSocket = accept(_server[i]->getSocketFd(), (struct sockaddr *)&clientAddr, &addrSize);
	if (clientSocket < 0){
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

	strcat(buffer, "HTTP/1.1 200 OK\n"
				"Server: myserv\n"
				"Content-Length: 5\n"
				"Connection: Keep-Alive\r\n\r\n");
	strcat(buffer, "a\r\n\r\n");

	int len = strlen(buffer);
	int s = send(pClient->getSocketFd(), buffer, len, 0);
	std::cout << "send = " << s << std::endl;

	bzero(buffer, len);
	pClient->setState(Client::State::REQUEST_PARSE);

}