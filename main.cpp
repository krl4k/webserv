#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <time.h>
#include <sys/socket.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#define FAILURE -1

// http://localhost:8000

#define SERVER_PORT    "8000"
#define MAX_CONNECTION 1000

int main() {

	int socketFD, clientSocketFD;
	socklen_t cliLen;
	char buffer[256];
	struct sockaddr_in serverAddress, clientAddress;
	int n;

	if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) == FAILURE)
		std::cerr << "Error opening socket" << std::endl;
	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(8080);
	if (bind(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == FAILURE)
		std::cerr << "Error on socker binding" << std::endl;
	listen(socketFD, 5);
	cliLen = sizeof(clientAddress);

	if ((clientSocketFD = accept(socketFD, (struct sockaddr *) &clientAddress, &cliLen)) == FAILURE)
		std::cerr << "Error on accept" << std::endl;
	bool flag = true;
	std::string msg;
	while (1)
	{
		if (flag) {
			std::cout << "Server: got connection from "  << inet_ntoa(clientAddress.sin_addr) << " port ";
			std::cout << ntohs(clientAddress.sin_port) << std::endl;
			flag = false;
			bzero(buffer, 256);
			send(clientSocketFD, "Hello from server\0", 18, 0);
		}
		bzero(buffer, 256);
		if ((n = recv(clientSocketFD, buffer, 255, 0)) == FAILURE)
			std::cerr << "Error reading from socket" << std::endl;
		std::cout << "Here is the message from client: " << buffer << std::endl;
		if (!flag) {
			std::cout << "Enter your message: ";
			getline(std::cin, msg);
			send(clientSocketFD, msg.c_str(), std::strlen(msg.c_str()), 0);
		}
	}
	close(clientSocketFD);
	close(socketFD);
	return 0;
}




