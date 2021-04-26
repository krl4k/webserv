// CLion
// Created by Foster Grisella on 4/13/21 15:01.
//


#include "../includes/WebServer.hpp"


WebServer::WebServer(const char *fileName){
	if (fileName)
		_configFileName = fileName;
	else
		_configFileName = "../configs/default.conf";
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
	for (size_t i = 0; i < _server.size(); ++i) {
		if (_server[i]->createSocket() < 0){
			std::cerr  << "Socket dont creating!" << std::endl;
			throw std::exception();
		}
	}
	lifeCycle();
}

WebServer::~WebServer() {
	for (size_t i = 0; i < _server.size(); ++i) {
		delete _server[i];
	}
	for (size_t i = 0; i < _client.size(); ++i) {
		delete _client[i];
	}
	_client.clear();
	_server.clear();
}


int WebServer::lifeCycle() {
	fd_set readFdSet, writeFdSet;
	_maxFdSize = _server.back()->getSocketFd();

	while (true) {
		std::cout << GREEN << "\nWaiting for connection! " << WHITE <<"Clients count = " << _client.size() << RESET << std::endl;
		initSocketSet(readFdSet, writeFdSet);

		select(_maxFdSize + 1, &readFdSet, &writeFdSet, NULL, NULL);

		acceptNewClient(readFdSet);
		handler(readFdSet, writeFdSet);
	}
}

void WebServer::initSocketSet(fd_set &readFdSet, fd_set &writeFdSet) {
	FD_ZERO(&readFdSet);
	FD_ZERO(&writeFdSet);
	for (size_t i = 0; i < _server.size(); ++i)
		FD_SET(_server[i]->getSocketFd(), &readFdSet);
	for (size_t i = 0; i < _client.size(); ++i) {
		FD_SET(_client[i]->getSocketFd(), &readFdSet);
		if (_client[i]->getState() != Client__State__REQUEST_PARSE){
			FD_SET(_client[i]->getSocketFd(), &writeFdSet);
		}
		if (_client[i]->getSocketFd() > _maxFdSize)
			_maxFdSize = _client[i]->getSocketFd();
	}
}

void WebServer::acceptNewClient(fd_set &readFdSet) {
	for (size_t i = 0; i < _server.size(); ++i) {
		if (FD_ISSET(_server[i]->getSocketFd(), &readFdSet)){
			Client *newClient = acceptNewConnection(i);
			if (newClient == nullptr) {
				std::cerr << "Accept error!" << std::endl;
				return;
			}
			_client.push_back(newClient);
			if (newClient->getSocketFd() > _maxFdSize)
				_maxFdSize = newClient->getSocketFd();
		}
	}
}

void WebServer::handler(fd_set &readFdSet, fd_set &writeFdSet) {
	for (size_t i = 0; i < _client.size(); ++i) {
		int fd = _client[i]->getSocketFd();
		if (FD_ISSET(fd, &readFdSet) && _client[i]->getState() == Client__State__REQUEST_PARSE) {
			readRequest(_client[i]);
			if (_client[i]->getState() != Client__State__CLOSE and \
					_client[i]->getRequest()->getState() == HttpRequest__State__FULL) {
				FD_SET(fd, &writeFdSet);
			}
		}
		if (FD_ISSET(fd, &writeFdSet) and _client[i]->getState() == Client__State__CREATING_RESPONSE){
			generateResponse(_client[i]);
		}
		if (FD_ISSET(fd, &writeFdSet) and _client[i]->getState() == Client__State__ACCEPT_RESPONSE) {
			sendResponse(_client[i]);

			if (_client[i]->getState() == Client__State__REQUEST_PARSE){
				_client[i]->getRequest()->clean();
				_client[i]->getResponse()->clean();
			}
		}
		if (_client[i]->getState() == Client__State__CLOSE) {
			std::vector<Client *>::iterator it = _client.begin() + (int)i;
				delete _client[i];
			_client.erase(it);
		}
	}
}

void WebServer::readRequest(Client *&client) {
	char buffer[BUFSIZE + 1];
	bzero(&buffer, BUFSIZE);
	ssize_t bytes_read;
	bytes_read = recv(client->getSocketFd(), buffer, BUFSIZE, 0);
#if DEBUG == 1
	std::cout << "bytes read = " << bytes_read << std::endl;
//	std::cout << "request:\n" << std::string(buffer, bytes_read) << std::endl;
#endif
	if (bytes_read <= 0) {
		client->setState(Client__State__CLOSE);
		return;
	}
	client->getRequest()->parse(buffer, bytes_read);
	if (client->getRequest()->getState() == HttpRequest__State__FULL){
		std::cout << MAGENTA<< "full" << RESET<< std::endl;
		client->setState(Client__State__CREATING_RESPONSE);
	}
}

Client *WebServer::acceptNewConnection(size_t serverNumber) {
	int clientSocket;
	struct sockaddr_in clientAddr;
	socklen_t addrSize = sizeof(sockaddr_in);
	clientSocket = accept(_server[serverNumber]->getSocketFd(), (struct sockaddr *)&clientAddr, &addrSize);
	SET_NONBLOCK(clientSocket);
	if (clientSocket < 0){
		return nullptr;
	}
	Client *client = new Client(clientSocket, _server[serverNumber]->getHost(), _server[serverNumber]->getPort(), clientAddr);
	std::cout 	<< YELLOW << "Client with ip " << RED << client->getInfo()
				<< YELLOW <<  " connected! " << MAGENTA << "FD = " << clientSocket << RESET << std::endl;
	return client;
}

void WebServer::generateResponse(Client *&client) {
	client->getResponse()->generate(client, findServer(client));
	client->setState(Client__State__ACCEPT_RESPONSE);
}

void WebServer::sendResponse(Client *&client) {
	ssize_t sendLen = client->getResponse()->getSendPos();

	char *buf = client->getResponse()->getCToSend();
	size_t len = static_cast<size_t>( client->getResponse()->getSendLen() - client->getResponse()->getSendPos());
	sendLen += send(client->getSocketFd(), &buf[client->getResponse()->getSendPos()], len, 0);
	std::cout << YELLOW << "Sending a response to the user with ip " << RED << client->getInfo()
	<< MAGENTA << ", len = " << sendLen - client->getResponse()->getSendPos()
	<< " FD = " << client->getSocketFd() <<  RESET << std::endl;

	client->getResponse()->setSendPos(sendLen);
	if (client->getResponse()->getSendLen() == client->getResponse()->getSendPos()){
		client->setState(Client__State__REQUEST_PARSE);
#if SEND_DEBUG == 1
		std::cout << "sending:\n" << client->getResponse()->getToSend() << std::endl;
#endif
	}
	if (client->getRequest()->getConnectionType() == "close") {
        client->setState(Client__State__CLOSE);
	}

}

Server *WebServer::findServer(Client *client){
	for (size_t i = 0; i < _server.size();++i){
		if (client->getHost() == _server[i]->getHost() && client->getPort() == _server[i]->getPort())
			return (_server[i]);
	}
	return (nullptr);
}


WebServer::WebServer() {

}

