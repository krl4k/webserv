#include <iostream>
#include <signal.h>
#include "WebServer.hpp"

static	void	sigHandler(int sig_num)
{
		signal(sig_num, sigHandler);
}

int main(int argc, char **argv) {
#if CONSOLE_MESSAGE_ON
	std::stringstream ss;
	std::cout.rdbuf (ss.rdbuf());       // <-- redirect
#endif
	if (argc <= 2){
		try {
			signal(SIGPIPE, sigHandler);
			signal(SIGILL, sigHandler);
			WebServer webServer(argv[1]);
			webServer.start();
		} catch (std::exception &exception) {
		    std::cerr << exception.what() << std::endl;
			std::cerr << "Server dont started!" << std::endl;
			return -1;
		}
	}
	return (0);
}