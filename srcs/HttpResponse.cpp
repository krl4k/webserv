// CLion
// Created by Foster Grisella on 4/15/21 10:01.
//


#include "../includes/HttpResponse.hpp"
#include "../includes/Client.hpp"
#include <sys/time.h>


HttpResponse::HttpResponse(){
	setStatusMessages();
	_error = "";
	_body = "";
	_buffer = "";
	_header_size = 0;
	_body_size = 0;
	_code = 0;
}

HttpResponse::~HttpResponse() {

}


// what generate need???


/**
 * todo check this 
 * void HttpResponse::generate(HttpRequest *&request) ???
 * or
 * void HttpResponse::generate(Client *&pClient) ???
 *
 */

bool checkMethod(std::vector<std::string> vec, std::string meth){
	for (size_t i = 0; i < vec.size(); ++i){
		if (meth == vec[i])
			return (true);
	}
	return (false);
}

void HttpResponse::generate(Client *client, Server *server) {
//	size_t strBuf;
//	char *buffer = (char *)calloc(BUFSIZE, sizeof (char));
//	strBuf = strlen(buffer);
//	_toSend.append("HTTP/1.1 200 OK\n");
//	_toSend.append("Server: obserVER\n");
//	_toSend.append("Content-Length: 5\n");
//	_toSend.append("Connection: Keep-Alive\r\n\r\n");
//
//	_toSend.append("a\r\n\r\n");
//	_toSend.append(buffer);
//	std::cout << "toSend = " << _toSend << std::endl;
//
//	free(buffer);
//	initResponse();

	std::string path = client->getRequest()->getPath();
	std::map<std::string, Location>::const_iterator it;
	it = server->getLocation().find(path);
	if (it == server->getLocation().end())
		_code = 404;
	Location ourLoc = it->second;
	std::string locName = it->first;

	if (!checkMethod(ourLoc.getAllowMethods(), client->getRequest()->getMethod())){
		_code = 405;
	}

	std::string root = ourLoc.getRoot();
	if (root[root.size() - 1] == '/')
		root.erase(root.size() - 1, 1);

	std::string mergedPath = root;
	if (locName[0] != '/'){
		locName = "";
	}
	mergedPath = root + '/' + locName;

	if (ourLoc.getClientMaxBodySize() > _body_size){
		_code = 413;
	}

}

void HttpResponse::setStatusMessages() {
	_status_messages[100] = "Continue";
	_status_messages[101] = "Switching Protocols";
	_status_messages[102] = "Processing";
	_status_messages[103] = "Early Hints";
	_status_messages[200] = "OK";
	_status_messages[201] = "Created";
	_status_messages[202] = "Accepted";
	_status_messages[203] = "Non-Authoritative Information";
	_status_messages[204] = "No Content";
	_status_messages[205] = "Reset Content";
	_status_messages[206] = "Partial content";
	_status_messages[207] = "Multy-Status";
	_status_messages[300] = "Multiple Choices";
	_status_messages[301] = "Moved Permanently";
	_status_messages[302] = "Mover Temporary";
	_status_messages[307] = "Temporary Redirect";
	_status_messages[308] = "Permanent Redirect";
	_status_messages[400] = "Bad Request";
	_status_messages[401] = "Unauthorized";
	_status_messages[403] = "Forbidden";
	_status_messages[404] = "Not Found";
	_status_messages[405] = "Method Not Allowed";
	_status_messages[406] = "Not Acceptable";
	_status_messages[411] = "Length Required";
	_status_messages[413] = "Rayload Too Large";
	_status_messages[500] = "Internal Server Error";
	_status_messages[501] = "Not Implemented";
}

std::string & HttpResponse::getStatusMessages(int n) {
	std::map<int, std::string>::iterator it = _status_messages.find(n);
	if (it == _status_messages.end())
		return (std::string &) ("Unknown");
	return (it->second);
}

std::string HttpResponse::getBody(int code, std::string &path) {
	std::stringstream buf;
	if (code < 400){
		char temp;
		int cor_fd = open(path.c_str(), O_RDONLY);
		while(read(cor_fd, &temp, 1) > 0){
			buf << temp;
		}
		close(cor_fd);
	}
	else{
		buf << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n"
			   "<html>\n"
			   " <head>\n"
			   "  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n"
			   "  <title>Пример веб-страницы</title>\n"
			   " </head>\n"
			   " <body>\n"
			   "  <h1>" << getStatusMessages(code) <<"</h1>\n"
													 " <h2>" << code << "</h2>\n"
																		" </body>\n"
																		"</html>";
	}
	return (buf.str());
}

std::string HttpResponse::createHeader(HttpRequest *req) {
	std::stringstream header;

	header << "HTTP/1.1 " << _code << " " << getStatusMessages(_code) << CRLF <<
		   "Date: " << getCurrentDate() << CRLF <<
		   "Server: " << "KiRoTa/6.9" << CRLF <<
		   "Content-type: " << req->getContentType() << CRLF <<
		   "Content-Length: " << _body_size;
	return (header.str());
}

void HttpResponse::setBody(std::string &body) {
	_body = body;
}


void HttpResponse::initResponse(HttpRequest *req, int code, std::string &path) {
	std::string head;
	_code = code;
	if (code != 0) {
		_error = getBody(code, path);
		_body_size = _error.length();
	}
	head = createHeader(req);
	if (!_error.empty())
		_body = _error;
	_toSend.append(head);
	_toSend.append("\r\n\r\n");
	_toSend.append(_body);
	_toSend.append("\r\n\r\n");
}

std::string HttpResponse::getCurrentDate() const {

	std::string date;
	time_t rawtime;
	time(&rawtime);
	date = ctime(&rawtime);
	return date;
}

const std::string &HttpResponse::getToSend() const {
	return _toSend;
}

void HttpResponse::clean() {
	_toSend.clear();
}

