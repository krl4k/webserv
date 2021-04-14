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

// http://localhost:8000

#include "includes/WebServer.hpp"

int main(int argc, char **argv) {
	if (argc <= 2){
		try {
			WebServer webServer(argv[1]);
		} catch (std::exception &exception) {
			std::cerr << "Server dont started!" << std::endl;
		}
	} else {
		std::cerr << "Wrong arguments!" << std::endl;
		return (-1);
	}
	return (0);
}




