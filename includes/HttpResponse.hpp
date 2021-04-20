// CLion
// Created by Foster Grisella on 4/15/21 10:01.
//


#ifndef WEBSERV_HTTPRESPONSE_HPP
#define WEBSERV_HTTPRESPONSE_HPP

#include "Defines.hpp"
class Client;
class Server;
#include "../includes/HttpRequest.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>


class HttpResponse {
public:
	HttpResponse();

	HttpResponse(const std::map<int, std::string> &statusMessages);

	std::string getCurrentDate() const;
	void generate(Client *client, Server *server);

	std::string & getStatusMessages(int n);
	void          setStatusMessages();

	virtual ~HttpResponse();

	HttpResponse(HttpResponse const &src);

	HttpResponse &operator=(HttpResponse const &src);

	std::string getBody(int code, std::string & path);
	std::string createHeader(HttpRequest * req);
	void		setBody(std::string & body);

	const std::string &getToSend() const;

	void initResponse(HttpRequest *req, int code, std::string & path);
	void	setStatusCode(int code);
	void clean();


private:
	std::string					_toSend;
	std::map<int, std::string>	_status_messages;
	std::string 			  	_body;
	std::string 			    _headers_cgi;
	int  						_body_size;
	int 						_header_size;
	std::string					_error;
	int							_code;
	std::string					_buffer;
};


#endif //WEBSERV_HTTPRESPONSE_HPP
