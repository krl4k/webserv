//
// Created by Magnemite Winter on 4/11/21.
//

#include "Client.hpp"

Client::Client(int socket, struct sockaddr_in address):
		_socket(socket), _address(address){}
