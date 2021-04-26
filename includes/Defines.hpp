// CLion
// Created by Foster Grisella on 4/18/21 11:46.
//


#ifndef WEBSERV1_DEFINES_HPP
#define WEBSERV1_DEFINES_HPP

#define CRLF "\r\n"
#define BODY_SEP "\r\n\r\n"
#define BUFSIZE 1000000
#define CGI_BUFSIZE 65535
#define closesocket(socket)  close(socket)
#define SOCKET	int
#define SET_NONBLOCK(socket) fcntl(socket, F_SETFL, O_NONBLOCK);

#define DEBUG 0
#define CGI_DEBUG 0
#define SEND_DEBUG 0
#define HTTP_REQUEST_DEBUG 0
#define CHUNKED_REQUEST_DEBUG 0



#endif //WEBSERV1_DEFINES_HPP
