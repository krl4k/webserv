// CLion
// Created by Foster Grisella on 4/10/21 13:03.
//

#include <iostream>
#include <string>
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

int main(int argc, char *argv[]) {

	struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];

	if (argc != 2) {
		std::cerr << "Usage: showip hostnamen" << std::endl;
		return 1;
	}
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((status = getaddrinfo(argv[1], NULL, &hints, &res))) {
		std::cerr << "getaddrinfo: " + std::string(gai_strerror(status)) << std::endl;
		return 2;
	}
	std::cout << hints.ai_socktype << " " << res->ai_socktype << std::endl;
	std::cout << "IP address for: " + std::string(argv[1]) << std::endl;
	for (p = res; p != nullptr; p = p->ai_next) {
		void *addr;
		std::string ipver;

		if (p->ai_family == AF_INET) {
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		std::cout << ipver << ": " << ipstr << std::endl;
	}
	freeaddrinfo(res);
	return 0;
}

//int main() {
//
//	char hostName[100];
//	size_t size;
//	int status;
//	hostent *tester;
//
//	memset(&hostName, 0, 100);
//	size = 100;
//	if ((status = gethostname(hostName, size)) == -1)
//		std::cerr << "Error" << std::endl;
//	std::cout << hostName << " " << size << std::endl;
//	tester = gethostbyname(hostName);
//	for (int i = 0; tester->h_addr_list[i]; i++)
//		std::cout <<"Address list: " << tester->h_addr_list[i] << std::endl;
//	std::cout << "Address type: " << tester->h_addrtype << std::endl;
//	for (int i = 0; tester->h_aliases[i] ; ++i)
//		std::cout << "Aliases: " << tester->h_aliases[i] << std::endl;
//	std::cout << "Address name: " << tester->h_name << std::endl;
//	std::cout << "Address length: " << tester->h_length << std::endl;
//}