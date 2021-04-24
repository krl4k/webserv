#include "../includes/CGI.hpp"

CGI::CGI(Server *server, Client *client, const char *path) {
	_request = client->getRequest();
	_response = client->getResponse();
	_path = strdup(path);
//	_environment = nullptr;
	setEnvironment(server, client);
	executeCGI(client);
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
//
//CGI::CGI(const CGI &other) {
//	*this = other;
//}

//char **CGI::clone(char **other) {
//	char **newString;
//
//	if (!(newString = (char **)calloc((sizeof(other) + 1), sizeof(char*))))
//		throw std::bad_alloc();
//	for (int i = 0; i < sizeof(other); i++)
//		if (!(newString[i] = strdup(other[i])))
//			throw std::bad_alloc();
//	return newString;
//
//}

//CGI &CGI::operator=(const CGI &other) {
//	if (this == &other)
//		return *this;
//	if (_environment) {
//		for (int i = 0; i < sizeof(_environment); i++)
//			if (_environment[i])
//				free(_environment[i]);
//		free(_environment);
//	}
//	if (_arguments)
//	{
//		for (int i = 0; i < sizeof(_arguments); i++)
//			if (_arguments[i])
//				free(_arguments[i]);
//		free(_arguments);
//	}
//	try {
//		_environment = clone(other._environment);
//		_arguments = clone(other._arguments);
//	} catch (std::exception &e) {
//		std::cerr << e.what() << std::endl;
//	}
//	_environmentSize = other._environmentSize;
//	if (!(_path = strdup(other._path)))
//		throw std::bad_alloc();
//	_response = other._response;
//	_request = other._request;
//	return *this;
//}

/**
 * Check http://www6.uniovi.es/~antonio/ncsa_httpd/cgi/env.html for environments.
 */
void CGI::setEnvironment(Server *server, Client *client) {
	std::map<std::string, std::string> env;

	env["AUTH_TYPE="] = "";								 //use to check user
	env["CONTENT_LENGTH="] = _request->getBody().size();// - The length of the said
														 // content as given by the client.
	env["CONTENT_TYPE"] = _request->getContentType();	 // - POST, GET, PUT
	env["GATEWAY_INTERFACE="] = "CGI/1.1";
	env["PATH_INFO="] = _request->getPath();				 // - The extra path information, as given by the client
														 //This information should be decoded by the server if it comes from a URL
	env["PATH_TRANSLATED="] = "";						 // - The server provides a translated version of PATH_INFO,
														 // which takes the path and does any virtual-to-physical mapping to it
	env["QUERY_STRING="] = _request->getQueryString();			 // - The information which follows the ? in the URL,
											   			 // It should not be decoded in any fashion
	env["REMOTE_ADDR="] = server->getHost();			 // - The IP address of the remote host making the request
	env["REMOTE_IDENT="] = "";							 // - name for remote user retrieved by server
														 // (server need supports RFC 931)
	env["REMOTE_USER="] = "";							 // - if server supports auth(login)
	env["REQUEST_METHOD="] = _request->getMethod();		 // - The method with which the request was made.
														 // For HTTP, this is "GET", "HEAD", "POST", etc.
	env["REQUEST_URI="] = _request->getPath();
	env["SCRIPT_NAME="] = _request->getPath();			 // - A virtual path to the script being executed,
														 // used for self-referencing URLs
	env["SERVER_NAME="] = server->getServerName();
	std::stringstream ss;
	ss << server->getPort();
	env["SERVER_PORT="] = std::string(ss.str());			 // - The port number to which the request was sent.
	env["SERVER_PROTOCOL="] = "HTTP/1.1";				 // - The name and revision of the information protocol
														 // this request came in with.
	env["SERVER_SOFTWARE="] = "KiRoTaMagic/6.9";
	setEnvToString(env);
	env.clear();
}

void CGI::setArguments() {
	_arguments = (char **)calloc(3, sizeof(char *));
	_arguments[0] = strdup(_path);
	_arguments[1] = strdup(_path);
}

char **CGI::setEnvToString(std::map<std::string, std::string> env) {
	_environment = (char **)calloc(env.size(), sizeof(char *));

	int i = 0;
	std::map<std::string, std::string>::iterator it;
	for (it = env.begin(); it != env.end(); it++) {
		std::string str = it->first + it->second;
		_environment[i] = strdup(str.c_str());
	}
	return _environment;
}

char **CGI::getEnvironment() const {return _environment;}

void	CGI::executeCGI(Client *client) {

	int 	pipeFd[2];
	int 	fileFd;
	pid_t	pid;
	char	*buffer;

	if (pipe(pipeFd) == -1) {
		_response->setStatusCode(500);
		throw std::runtime_error(RED + std::string("Pipe fail") + RESET);
	}
	if ((fileFd = open("file", O_CREAT | O_RDWR | O_TRUNC, 0666)) == -1)
		throw std::runtime_error(RED + std::string("can't open file in executCGI") + RESET);
	if ((pid = fork()) == -1)
		throw std::runtime_error(RED + std::string("fork failure") + RESET);
	else if (!pid) { 															//child process:D It's fckng magic:D
		close(pipeFd[1]);
		dup2(pipeFd[0], 0);
		close(pipeFd[0]);
		dup2(fileFd, 1);
		close(fileFd);
		exit(execve(_arguments[0], _arguments, _environment));
	} else {
		write(pipeFd[1], _request->getBody().c_str(), _request->getBody().length());
		close(pipeFd[1]);
		close(pipeFd[0]);
		int state;
		wait(&state);
		if (!state) {
			lseek(fileFd, SEEK_SET, SEEK_SET);
			if (!(buffer = (char *)calloc(2, sizeof(char))))
				throw std::bad_alloc();
			for (int bytes; bytes > 0; bytes = read(fileFd, buffer, 1)) {
				int size = strlen(buffer);
				if (!(buffer = (char *)realloc(buffer, size + 1)))
					throw std::bad_alloc();
				buffer[size + 1] = '\0';
			}
		}

		std::string bufStr(buffer);
		std::string cgiHeader = bufStr.substr(0, bufStr.find(BODY_SEP) + 4);
		std::string cgiBody = bufStr.substr(bufStr.find(BODY_SEP) + 4, std::string::npos);
		_response->setCGIHeader(cgiHeader);
		_response->setBody(cgiBody);
		_response->setStatusCode(200);
		close(fileFd);
		free(buffer);
	}
}