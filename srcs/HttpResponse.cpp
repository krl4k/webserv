// CLion
// Created by Foster Grisella on 4/15/21 10:01.
//


#include "../includes/HttpResponse.hpp"
#include "../includes/Client.hpp"
#include <sys/time.h>


HttpResponse::HttpResponse() {
	setStatusMessages();
	_error = "";
	_body = "";
	_buffer = "";
	_header_size = 0;
	_body_size = 0;
	_code = 0;
	_newCGI = new CGI();
}

HttpResponse::~HttpResponse() {
	delete _newCGI;
}


bool checkMethod(std::vector<std::string> vec, std::string meth) {
	for (size_t i = 0; i < vec.size(); ++i) {
		if (meth == vec[i])
			return (true);
	}
	return (false);
}

void HttpResponse::checkFile(Location &ourLoc, std::string &mergedPath, struct stat *fileInfo) {
	int fd = 0;

	/** Checking access mode */
	if (S_ISDIR(fileInfo->st_mode)) {
		if (!ourLoc.getIndex().empty()) { mergedPath = mergedPath + '/' + ourLoc.getIndex(); }
		if ((fd = open(mergedPath.c_str(), O_RDONLY)) < 0) { _code = 404; }
		close(fd);
		stat(mergedPath.c_str(), fileInfo);
	}
	/*else{	std::cout << "File access error" << std::endl;
		_code = 500;
	}*/
}

int HttpResponse::getCode() const {
	return _code;
}

void HttpResponse::createPutResponse(Client *client, struct stat fileInfo, std::string &mergedPath, int flag) {
	int fd = 0;
	char buff;
	int bytes = 0;

	if ((fd = open(mergedPath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0755)) < 0) { _code = 500; }
	else if (S_ISDIR(fileInfo.st_mode)) {
		_code = 404;
		return;
	} else {
		write(fd, client->getRequest()->getBody().c_str(), client->getRequest()->getBody().size());
		if (flag != -1) { _code = 200; }
		else { _code = 201; }
	}
	close(fd);
	std::cout << "Put request" << std::endl;
}

std::string HttpResponse::bodyResponceInit(std::string &mergedPath) {
	char buff[10000];
	int fd = 0;
	std::stringstream temp;
	ssize_t res = 0;

	if (!(fd = open(mergedPath.c_str(), O_RDONLY))) { std::cerr << "Can't open file" << std::endl; }
	size_t size = 10000;
	while ((res = read(fd, &buff, size)) > 0) {
		buff[res] = '\0';
		temp << buff;
	}
	close(fd);
	return (temp.str());
}


void HttpResponse::createGetOrHead(Client *client, struct stat fileInfo, Location &ourLoc, std::string &mergedPath,
								   std::string errorPage, int errorPageCode) {

	if ((S_ISLNK(fileInfo.st_mode) || S_ISREG(fileInfo.st_mode))) {
		if (client->getRequest()->getMethod() == "GET") {
			std::string temp = bodyResponceInit(mergedPath);
			this->setBody(temp);
			if (temp.size() > _maxBodySize && _maxBodySize != 0) {
				_code = 413;
				_isThereErrorPage = (_configErrorCode == 413) ? -1 : 1;
			}
		}
	} else if (S_ISDIR(fileInfo.st_mode) && !ourLoc.getAutoIndex()) { _code = 404; }
	else if (S_ISDIR(fileInfo.st_mode) && client->getRequest()->getMethod() == "GET" && ourLoc.getAutoIndex()) {
	} else { _code = 404; }

	if (_code == errorPageCode) {
		int status = 0;
		size_t n;
		if ((n = (mergedPath.rfind('/'))) == std::string::npos) {

		}
		std::string tempPath = mergedPath.substr(0, n) + '/' + errorPage;
		mergedPath = tempPath;
		status = stat(mergedPath.c_str(), &fileInfo);
		_isThereErrorPage = (status != -1) ? 1 : 2;
	}
}

size_t find_loc(std::string loc) {
	ssize_t n = std::count(loc.begin(), loc.end(), '/');
	size_t pos = 0;

	if (n >= 2) {
		while (std::count(loc.begin(), loc.end(), '/') > 1) {
			pos = static_cast<size_t>(loc.rfind('/'));
			if (pos == 0) { break; }
			loc = loc.substr(0, pos);
		}
	}
	return (pos);
}

void HttpResponse::generate(Client *client, Server *server) {
	struct stat fileInfo;
	std::string mergedPath;
	std::string root;
	std::string tmpIndex;
	int flag = 0;
/**
 * Need fix location parser
 */
	std::string path = client->getRequest()->getPath();
	if (path[path.size() - 1] == '/' && path.size() > 1)
		path.erase(path.size() - 1, 1);
	std::map<std::string, Location>::const_iterator it;
	size_t pos = path.size();
	pos = find_loc(path);
	if (pos == 0) {
		it = server->getLocation().find(path);
		pos = (it == server->getLocation().end()) ? 1 : path.size();
	}
	tmpIndex = path.substr(pos, std::string::npos);
	path = path.substr(0, pos);


	it = server->getLocation().find(path);
	_code = 200;
	_configErrorCode = server->getErrorPageCode();
	/*if (path == "/directory" && tmpIndex == "/Yeah")
		_code = 404;*/
	Location ourLoc;
	if (it == server->getLocation().end() || !isAuthClient(client, server)) {
		if (!isAuthClient(client, server)) { _code = 403; }
		else { _code = 404; }
		path = "";
		mergedPath = server->getErrorPage();
		if (mergedPath.empty()) {
			_isThereErrorPage = -1; /* Means that there is no errorPage set */
		} else if (server->getErrorPageCode() != _code) {
			_isThereErrorPage = -1;
		} else {
			mergedPath = RESOURCES_PATH + mergedPath;
			_isThereErrorPage = 1;
		}
	} else {
		ourLoc = it->second;
		std::string locName = it->first;
		_maxBodySize = ourLoc.getClientMaxBodySize();

		if (!checkMethod(ourLoc.getAllowMethods(), client->getRequest()->getMethod())) { _code = 405; }

		root = ourLoc.getRoot();
		if (root[root.size() - 1] == '/')
			root.erase(root.size() - 1, 1);
		mergedPath = root;
		if (locName[0] != '/') { locName = ""; }
		mergedPath += tmpIndex;

		flag = stat(mergedPath.c_str(), &fileInfo);
		if (_code < 400) {
			checkFile(ourLoc, mergedPath, &fileInfo);
			if (client->getRequest()->getMethod() == "POST") {
				if (!ourLoc.getCgiPath().empty()) {
					std::cout << "---CGI---" << std::endl;
					std::string cgi = ourLoc.getCgiPath();
					if (cgi.find(".bla", 0, 4) != std::string::npos) {
						size_t i = cgi.find(".bla", 0, 4) + 4;
						for (; i < cgi.size() && (cgi[i] == ' ' || cgi[i] == '\t'); ++i);
						std::string temp = cgi.substr(i, cgi.size() - i);
						try {
							_newCGI->init(server, client, temp.c_str());
						} catch (std::exception &e) {
							std::cout << e.what() << std::endl;
						}
					}
				} else {
					createPutResponse(client, fileInfo, mergedPath, flag);
				}
			}
			if (client->getRequest()->getMethod() == "GET" || client->getRequest()->getMethod() == "HEAD") {
				createGetOrHead(client, fileInfo, ourLoc, mergedPath, server->getErrorPage(),
								server->getErrorPageCode());
			} else if (client->getRequest()->getMethod() == "PUT") {
				createPutResponse(client, fileInfo, mergedPath, flag);
			}
		}
	}
	initResponse(client->getRequest(), mergedPath);

}

void HttpResponse::setBodySize(size_t bodySize) {
	_body_size = bodySize;
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

void HttpResponse::setStatusCode(int code) {
	_code = code;
}

std::string &HttpResponse::getStatusMessages(int n) {
	std::map<int, std::string>::iterator it = _status_messages.find(n);
	if (it == _status_messages.end())
		return (std::string &) ("Unknown");
	return (it->second);
}

std::string HttpResponse::getPage(std::string &path) {
	std::stringstream buf;

	if (_code > 400 && _isThereErrorPage == -1) {
		/** If there is no error page*/
		buf << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n"
			   "<html>\n"
			   " <head>\n"
			   "  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n"
			   "  <title>Пример веб-страницы</title>\n"
			   " </head>\n"
			   " <body>\n"
			   "  <h1>" << getStatusMessages(_code) << "</h1>\n" << "_____________________" <<
			" <h2>" << _code << "</h2>\n"
								" </body>\n"
								"</html>";
	} else {
		char temp;
		int cor_fd = open(path.c_str(), O_RDONLY);
		if (!cor_fd)
			throw std::runtime_error("Can't open file");
		while (read(cor_fd, &temp, 1) > 0) {
			buf << temp;
		}
		close(cor_fd);
	}
	return (buf.str());
}

std::string HttpResponse::createHeader(HttpRequest *req) {
	std::stringstream header;

	header << "HTTP/1.1 " << _code << " " << getStatusMessages(_code) << CRLF <<
		   "Date: " << getCurrentDate() << CRLF <<
		   "Server: " << "KiRoTa/0.1" << CRLF <<
		   "Content-type: " << req->getContentType() << CRLF <<
		   "Content-Length: " << _body_size << BODY_SEP;
	return (header.str());
}

void HttpResponse::setBody(std::string &body) {
	_body = body;
}

void HttpResponse::initResponse(HttpRequest *req, std::string &path) {
	if (_body.empty())
		_body = getPage(path);
	_body_size = _body.length();
//	std::cout << _maxBodySize << std::endl;
	if (_body_size > _maxBodySize && _maxBodySize != 0) {
		_code = 413;
		_isThereErrorPage = (_configErrorCode == 413) ? 1 : -1;
		_body = getPage(path);
		_body_size = _body.length();
	}
	_toSend.append(createHeader(req));
	_toSend.append(_body);
	setCToSend();
}

std::string HttpResponse::getCurrentDate() const {

	std::string date;
	time_t rawtime;
	time(&rawtime);
	date = ctime(&rawtime);
	date.erase(date.size() - 1);
	return date;
}

const std::string &HttpResponse::getToSend() const {
	return _toSend;
}

void HttpResponse::clean() {
	_toSend.clear();
//	free(_c_toSend);
	_body.clear();
	if (_newCGI && _newCGI->isInit())
		_newCGI->clean();
}

const std::string &HttpResponse::getBody() const {
	return _body;
}

bool HttpResponse::isAuthClient(Client *pClient, Server *pServer) {
	if (pServer->getAuthBasicUserFile().empty()) { return true; }
	std::map<std::string, std::string>::const_iterator it(pClient->getRequest()->getHeaders().find("AUTHORIZATION"));
	if (it != pClient->getRequest()->getHeaders().end()) {
		size_t startEncode = it->second.find(" ") + 1;
		std::string encode(it->second, startEncode, it->second.size() - startEncode);
		std::string decode = base64_decode(encode);
		for (size_t i = 0; i < pServer->getWhiteList().size(); ++i) {
			if (decode == pServer->getWhiteList()[i]) {
				return true;
			}
		}
	}
	return false;
}

void HttpResponse::setCgiHeader(std::string header) {
	if (_newCGI->isInit())
		_cgiHeader = header;
}

void HttpResponse::setCode(int code) {
	_code = code;
}

char *HttpResponse::getCToSend() const {
	return _c_toSend;
}

void HttpResponse::setCToSend() {
	_sendLen = static_cast<ssize_t>(_toSend.size());
	_sendPos = 0;
	_c_toSend = const_cast<char *>(_toSend.c_str());
}

ssize_t HttpResponse::getSendPos() const {
	return _sendPos;
}

void HttpResponse::setSendPos(ssize_t sendPos) {
	_sendPos = sendPos;
}

ssize_t HttpResponse::getSendLen() const {
	return _sendLen;
}
