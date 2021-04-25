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
	for (int i = 0; i < _server.size(); ++i) {
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

//	struct timeval timeout;
	while (true) {
//		timeout.tv_sec = 10;
//		timeout.tv_usec = 1;
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
			_client.push_back(newClient);
			if (newClient->getSocketFd() > _maxFdSize)
				_maxFdSize = newClient->getSocketFd();
		}
	}
}

void WebServer::handler(fd_set &readFdSet, fd_set &writeFdSet) {
	for (int i = 0; i < _client.size(); ++i) {
		int fd = _client[i]->getSocketFd();
		if (FD_ISSET(fd, &readFdSet) && _client[i]->getState() == Client::State::REQUEST_PARSE) {
			readRequest(_client[i]);
			if (_client[i]->getState() != Client::State::CLOSE and \
					_client[i]->getRequest()->getState() == HttpRequest::State::FULL) {
				FD_SET(fd, &writeFdSet);
			}
		}
		if (FD_ISSET(fd, &writeFdSet) and _client[i]->getState() == Client::State::CREATING_RESPONSE){
			generateResponse(_client[i]);
		}
		if (FD_ISSET(fd, &writeFdSet) and _client[i]->getState() == Client::State::ACCEPT_RESPONSE) {
			sendResponse(_client[i]);
//			if response sending all
//			_client[i]->setState(Client::State::REQUEST_PARSE);
//
			//todo clint->clean
			_client[i]->setState(Client::State::REQUEST_PARSE);
			_client[i]->getRequest()->clean();
			_client[i]->getResponse()->clean();
		}
		if (_client[i]->getState() == Client::State::CLOSE) {
			std::vector<Client *>::iterator it = _client.begin() + i;
				delete _client[i];
			_client.erase(it);

		}
	}
}

void WebServer::readRequest(Client *&client) {
	char buffer[BUFSIZE + 1];
	bzero(&buffer, BUFSIZE);
	int bytes_read;
	bytes_read = recv(client->getSocketFd(), buffer, BUFSIZE, 0);
#if DEBUG == 1
	std::cout << "bytes read = " << bytes_read << std::endl;
	std::cout << "request:\n" << std::string(buffer, bytes_read) << std::endl;
#endif
	if (bytes_read <= 0) {
		client->setState(Client::State::CLOSE);
		return;
	}
	client->getRequest()->parse(buffer, bytes_read);
	if (client->getRequest()->getState() == HttpRequest::State::FULL){
		std::cout << MAGENTA<< "full" << RESET<< std::endl;
		client->setState(Client::State::CREATING_RESPONSE);
//		client->getRequest()->setState(HttpRequest::ParserState::QUERY_STRING);
	}
}

Client *WebServer::acceptNewConnection(int serverNumber) {
	int clientSocket;
	struct sockaddr_in clientAddr;
	socklen_t addrSize = sizeof(sockaddr_in);
	clientSocket = accept(_server[serverNumber]->getSocketFd(), (struct sockaddr *)&clientAddr, &addrSize);
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
	client->setState(Client::State::ACCEPT_RESPONSE);
}

void WebServer::sendResponse(Client *&client) {
	const char *buffer = client->getResponse()->getToSend().c_str();

	size_t len = strlen(buffer);
	std::cout << "len buffer to send = " << len << std::endl;

	size_t responseLen = client->getResponse()->getToSend().size();

	size_t sendLen = 0;

	//todo rewrite
	std::cout << "Before send: " << responseLen << std::endl;
	while (sendLen != responseLen) {
		sendLen += send(client->getSocketFd(), buffer, len, 0);
	}
	std::cout << "After send" << std::endl;
	std::cout << "Send Len: " << sendLen << std::endl;
	std::cout << YELLOW << "Sending a response to the user with ip " << RED << client->getInfo() << "."
			  << MAGENTA << " FD = " << client->getSocketFd() << RESET << std::endl;
	// if (response fully send)

}


//std::string readFile(const std::string& fileName) {
//	std::ifstream f(fileName);
//	f.seekg(0, std::ios::end);
//	size_t size = f.tellg();
//	std::string s(size, ' ');
//	f.seekg(0);
//	f.read(&s[0], size); // по стандарту можно в C++11, по факту работает и на старых компиляторах
//	return s;
//}

Server *WebServer::findServer(Client *client){
	for (size_t i = 0; i < _server.size();++i){
		if (client->getHost() == _server[i]->getHost() && client->getPort() == _server[i]->getPort())
			return (_server[i]);
	}
	return (nullptr);
}


WebServer::WebServer() {

}

