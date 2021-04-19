// CLion
// Created by Foster Grisella on 4/15/21 10:01.
//


#include "../includes/HttpResponse.hpp"
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

void HttpResponse::generate() {

	char *buffer = (char *)calloc(BUFSIZE, sizeof (char));
	strcat(buffer, "HTTP/1.1 200 OK\n"
				   "Server: myserv\n"
				   "Content-Length: 5\n"
				   "Connection: Keep-Alive\r\n\r\n");
	strcat(buffer, "a\r\n\r\n");

	_toSend.append(buffer);

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

std::string HttpResponse::getErrorPage(int code, std::string &path) {
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
		   "Content-Length: " << _body_size << CRLF;
	return (header.str());
}

void HttpResponse::setBody(std::string &body) {
	_body = body;
}


void HttpResponse::initResponse(HttpRequest *req, int code, std::string &path) {
//	std::string head;
//	_code = code;
//	if (code != 0) {
//		_error = getErrorPage(code, path);
//		_body_size = _error.length();
//	}
//	head = createHeader(req);
//	if (!_error.empty())
//		_body = _error;
//	_req_to_send.append(head);
//	_req_to_send.append("\r\n");
//	_req_to_send.append(_body);
//	_req_to_send.append("\r\n");
}

std::string &HttpResponse::getCurrentDate() const {
	std::string date;
	struct timeval tv;
	time_t t;
	struct tm *info;
	char buffer[64];

	gettimeofday(&tv, NULL);
	t = tv.tv_sec;

	info = localtime(&t);
	printf("%s",asctime (info));
	strftime (buffer, sizeof buffer, "Today is %A, %B %d.\n", info);
	printf("%s",buffer);
	strftime (buffer, sizeof buffer, "The time is %I:%M %p.\n", info);
	printf("%s",buffer);

	//todo some shit
	//date = "11 11 11";

	return date;
}

char *HttpResponse::getFinalResponse() const{
	return (_req_to_send);
}
