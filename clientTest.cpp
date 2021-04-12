#include <stdio.h>
#include <stdlib.h>
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
#include <string>
#include <ios>
#include <iostream>
#define FAILURE -1
int main() {

	int socketFD;
	struct sockaddr_in servAddress;
	struct hostent *server;
	char  buffer[256];

	if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) == FAILURE)
		std::cerr << " Error on opening socket" << std::endl;
	server = gethostbyname("myHost");
	bzero(&servAddress, sizeof(servAddress));
	servAddress.sin_family = AF_INET;
//	bcopy(server->h_addr_list, &servAddress.sin_addr.s_addr, server->h_length);
	servAddress.sin_port = htons(8080);
	if (connect(socketFD, (struct sockaddr *)&servAddress, sizeof(servAddress)) == FAILURE)
		std::cerr << "Error connecting" <<std::endl;
	std::cout << "Please enter your message: ";
	bzero(buffer, 256);
	fgets(buffer, 255, stdin);
	if (write(socketFD, buffer, std::strlen(buffer)) == FAILURE)
		std::cerr << "Error writing to socket" <<std::endl;
	bzero(buffer, 256);
	if (read(socketFD, buffer, 255) == FAILURE)
		std::cerr << "Error reading from socket" << std::endl;
	std::cout << buffer << std::endl;
	close(socketFD);
	return 0;
}
