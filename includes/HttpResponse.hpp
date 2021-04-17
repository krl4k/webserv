// CLion
// Created by Foster Grisella on 4/15/21 10:01.
//


#ifndef WEBSERV_HTTPRESPONSE_HPP
#define WEBSERV_HTTPRESPONSE_HPP

#include <string>
#include <iostream>
#include "../includes/HttpRequest.hpp"
#include <map>
#define RN "\r\n"
#include <sstream>
#include <unistd.h>
#include <string>
#include <fcntl.h>

class HttpResponse {
public:
	HttpResponse();

	HttpResponse(const std::map<int, std::string> &statusMessages);

	std::string &getCurrentDate() const;
	void generate();

	std::string & getStatusMessages(int n);
	void          setStatusMessages();

	virtual ~HttpResponse();

	HttpResponse(HttpResponse const &src);

	HttpResponse &operator=(HttpResponse const &src);

	std::string getErrorPage(int code, std::string & path);
	std::string createHeader(HttpRequest * req);
	void		setBody(std::string & body);

	std::string &getFinalResponse(){
		return (_req_to_send);
	}

	void initResponse(HttpRequest *req, int code, std::string & path);


private:
	std::map<int, std::string> _status_messages;
	std::string 			   _body;
	std::string 			    _headers_cgi;
	int  						_body_size;
	int 						_header_size;
	std::string					_error;
	int							_code;
	std::string					_buffer;
	std::string					_req_to_send;

};


#endif //WEBSERV_HTTPRESPONSE_HPP
