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

	int i;
	char **env = (char **)calloc(sizeof(char *), 18);
	for (i = 0; i < 18; i++)
	{
		env[i] = strdup("ab");
	}
	std::cout << i << std::endl;
	env[i] = nullptr;
	for (i = 0; env[i]; i++);
	std::cout << i << std::endl;
}




