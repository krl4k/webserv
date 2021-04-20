#include <iostream>

#include "includes/WebServer.hpp"

int main(int argc, char **argv) {
	if (argc <= 2){
		try {
			WebServer webServer(argv[1]);
			webServer.start();
		} catch (std::exception &exception) {
			std::cerr << "Server dont started!" << std::endl;
			return -1;
		}
	} else {
		try {
			WebServer webServer("./configs/default.conf");
			webServer.start();
		} catch (std::exception &exception) {
			std::cerr << "Server dont started!2" << std::endl;
			return -1;
		}
	}
	return (0);
}




