// CLion
// Created by Foster Grisella on 4/15/21 10:01.
//


#ifndef WEBSERV_HTTPRESPONSE_HPP
#define WEBSERV_HTTPRESPONSE_HPP

#include "Defines.hpp"
class Client;
class Server;
class CGI;
#include "../includes/HttpRequest.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/CGI.hpp"
#include "../includes/base64.h"

#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


#define RESOURCES_PATH "../resources/"
#define ERROR_PAGE_PATH "../resources/error.html"
#define ERROR_PAGE		"../html/"

class HttpResponse {
public:
	HttpResponse();

	HttpResponse(const std::map<int, std::string> &statusMessages);

	std::string getCurrentDate() const;
	void generate(Client *client, Server *server);

	std::string & getStatusMessages(int n);
	void          setStatusMessages();

	void	setStatusCode(int code);
	virtual ~HttpResponse();

	HttpResponse(HttpResponse const &src);

	HttpResponse &operator=(HttpResponse const &src);

	char *getPage(std::string &path);
	std::string createHeader(HttpRequest * req, Client *&client);
	void		setBody(char* body);
	void		setBodySize(size_t bodySize);
	const std::string &getToSend() const;
	void initResponse(HttpRequest *req, std::string &path, Client *&client);
	void clean();
	void setCgiHeader(std::string header);
	void checkFile(Location &ourLoc, std::string &mergedPath, struct stat *fileInfo);
	void createPutResponse(Client *client,  struct stat fileInfo, std::string &mergedPath, int flag);
	void createGetOrHead(Client *client, struct stat fileInfo, Location &ourLoc, std::string &mergedPath, std::string errorPage, int errorPageCode);
	void bodyResponceInit(std::string &mergedPath);
	int getCode() const;
	void setCode(int code);
	const std::string &getBody() const;
	void setContentType(std::string contentType);
	char *getCToSend() const;
	void setCToSend();
	ssize_t getSendLen() const;
	ssize_t getSendPos() const;
	void setSendPos(ssize_t sendPos);

private:
	std::string					_toSend;
	char *						_c_toSend;
	ssize_t 						_sendLen;
	ssize_t 						_sendPos;
	std::map<int, std::string>	_status_messages;
	char        * 			  	_body;
	std::string					_cgiHeader;
	std::string 			    _contentType;
	size_t  						_body_size;
	int 						_configErrorCode;
	size_t 						_header_size;
public:
    size_t getHeaderSize() const;

    void setHeaderSize(size_t headerSize);

private:
    std::string					_error;
	int							_code;
	int 						_isThereErrorPage;
	size_t							_maxBodySize;
	std::string					_buffer;
	std::string getAutoIndexPage(std::string &path);
	CGI							*_newCGI;
	bool isAuthClient(Client *pClient, Server *pServer);
};


#endif //WEBSERV_HTTPRESPONSE_HPP
