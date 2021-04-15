#include "CGI.hpp"

CGI::CGI(Client *client, char *path) {
	_request = client.getRequest();
	_response = client.getResponse();
	_path = path;
	setEnvironment();
}

CGI::~CGI() {
	for (int i = 0; i < sizeof(_environment); i++)
		free(_environment[i]);
	free(_environment);
	for (int i = 0; i < sizeof(_arguments); i++)
		free(_arguments[i]);
	free(_arguments);
	free(_path);
}

CGI::CGI(const CGI &other) {
	*this = other;
}

CGI &CGI::operator=(const CGI &other) {
	if (this == &other)
		return *this;
	_environment.clear();
	_arguments.clear();
	_environment = other._environment;
	_arguments = other._arguments;
	_environmentSize = other._environmentSize;
	_path = other._path;
	_response = other._response;
	_request = other._request;
	return *this;
}

void CGI::mapToString(std::map <std::string, std::string> env) {
	_environment = new char *[env.size() + 1]();

	int i = 0;
	for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); it++) {
		std::string str = it->first + it->second;
		_environment[i] = strdup(str.c_str());
	}
}

/**
 * Check http://www6.uniovi.es/~antonio/ncsa_httpd/cgi/env.html for environments.
 */
void CGI::setEnvironment() {
	std::map<std::string, std::string> env;

	env["AUTH_TYPE="] = "";								 //use to check user
	env["CONTENT_LENGTH="] = _request.getContentLength();// - The length of the said
														 // content as given by the client.
	env["CONTENT_TYPE"] = _request.getContentType();	 // - POST, GET, PUT
	env["GATEWAY_INTERFACE="] = "CGI/1.1";
	env["PATH_INFO="] = _request.getPath();				 // - The extra path information, as given by the client
														 //This information should be decoded by the server if it comes from a URL
	env["PATH_TRANSLATED="] = "";						 // - The server provides a translated version of PATH_INFO,
														 // which takes the path and does any virtual-to-physical mapping to it
	env["QUERY_STRING="] = _request.getQuery();			 // - The information which follows the ? in the URL,
											   			 // It should not be decoded in any fashion
	env["REMOTE_ADDR="] = client.getIP();				 // - The IP address of the remote host making the request
	env["REMOTE_IDENT="] = "";							 // - name for remote user retrieved by server
														 // (server need supports RFC 931)
	env["REMOTE_USER="] = "";							 // - if server supports auth(login)
	env["REQUEST_METHOD="] = _request.getMethod();		 // - The method with which the request was made.
														 // For HTTP, this is "GET", "HEAD", "POST", etc.
	env["REQUEST_URI="] = _request.getUri();
	env["SCRIPT_NAME="] = _request.getPath();			 // - A virtual path to the script being executed,
														 // used for self-referencing URLs
	env["SERVER_NAME="] = _request.getServeName();
	env["SERVER_PORT="] = _request.getPort();			 // - The port number to which the request was sent.
	env["SERVER_PROTOCOL="] = "HTTP/1.1";				 // - The name and revision of the information protocol
														 // this request came in with.
	env["SERVER_SOFTWARE="] = "MwintersMagic/0.1";
	mapToString(env);
	env.clear();
}

char *CGI::strjoin(char *s1, char *s2) {

	char *res;
	int i = 0;

	if (!s1 || !s2)
		return nullptr;
	if (!(res = (char *)malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1))))
		return nullptr;
	while (*s1)
		res[i++] = *s1++;
	while (*s2)
		res[i++] = *s2++;
	res[i] = '\0';
	return res;
}

bool CGI::checkAlloc() {
	int i;

	for (i = 0; _environment[i]; i++);
	if (i != _environmentSize)
		return false;
	return true;
}

void CGI::setArguments() {
	_arguments.push_back(_path);
	_arguments.push_back(_path);
}

char **CGI::getEnvironment() const {return _environment;}