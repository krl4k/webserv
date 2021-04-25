#include <iostream>

#include "includes/WebServer.hpp"

static	void	sigHandler(int sig_num)
{
		signal(sig_num, sigHandler);
}

int main(int argc, char **argv) {
	if (argc <= 2){
		try {
			signal(SIGPIPE, sigHandler);
			signal(SIGILL, sigHandler);
			WebServer webServer(argv[1]);
			webServer.start();
		} catch (std::exception &exception) {
			std::cerr << "Server dont started!" << std::endl;
			return -1;
		}
	}
	return (0);
}