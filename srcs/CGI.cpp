#include "../includes/CGI.hpp"

CGI::CGI() {
	_init = false;
}

void CGI::init(Server *server, Client *client, const char *path) {
	_request = client->getRequest();
	_response = client->getResponse();
	_path = strdup(path);
	_bodySize = 0;
	_init = true;
	setEnvironment(server);
	executeCGI();
}

CGI::~CGI() {

}

bool CGI::isInit() {return _init;}

/**
 * Check http://www6.uniovi.es/~antonio/ncsa_httpd/cgi/env.html for environments.
 */
void CGI::setEnvironment(Server *server) {
	std::map<std::string, std::string> env;

	//env["REDIRECT_STATUS"] = "200";
	env["AUTH_TYPE"] = "basic";								 //use to check user
	env["CONTENT_LENGTH"] = std::to_string(_request->getBody().size());// - The length of the said
														 // content as given by the client.
	env["CONTENT_TYPE"] = _request->getContentType();	 // - POST, GET, PUT
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["PATH_INFO"] = _request->getPath();				 // - The extra path information, as given by the client
														 //This information should be decoded by the server if it comes from a URL
	env["PATH_TRANSLATED"] = _request->getPath();						 // - The server provides a translated version of PATH_INFO,
														 // which takes the path and does any virtual-to-physical mapping to it
	env["QUERY_STRING"] = _request->getPath() + "?" + _request->getQueryString();			 // - The information which follows the ? in the URL,
											   			 // It should not be decoded in any fashion
	env["REMOTE_ADDR"] = server->getHost();			 // - The IP address of the remote host making the request
	env["REMOTE_IDENT"] = "basic";							 // - name for remote user retrieved by server
														 // (server need supports RFC 931)
	env["REMOTE_USER"] = "basic";							 // - if server supports auth(login)
	env["REQUEST_METHOD"] = _request->getMethod();		 // - The method with which the request was made.
	env["REQUEST_URI"] = _request->getPath();
	env["SCRIPT_NAME"] = _request->getPath();			 // - A virtual path to the script being executed,
														 // used for self-referencing URLs
	if (server->getServerName().empty())
		env["SERVER_NAME"] = env["REMOTE_ADDR"];
	else
		env["SERVER_NAME"] = server->getServerName();
	env["SERVER_PORT"] = std::to_string(server->getPort());			 // - The port number to which the request was sent.
	env["SERVER_PROTOCOL"] = "HTTP/1.1";				 // - The name and revision of the information protocol
														 // this request came in with.
	env["SERVER_SOFTWARE"] = "KiRoTaMagic/6.9";
	env["HTTP_X_SECRET_HEADER_FOR_TEST"] = "1";
	env.insert(_request->getHeaders().begin(), _request->getHeaders().end());
	setEnvToString(env);
	env.clear();
}


/**
 * rotate env from map to string for execve
 * @param std::map<>env
 * @return string of env
 */
char **CGI::setEnvToString(std::map<std::string, std::string> env) {
	_environment = (char **)calloc(env.size() + 1, sizeof(char *));

	std::map<std::string, std::string>::iterator it;
	int i = 0;
	for (it = env.begin(); it != env.end(); it++, i++) {
		std::string str = it->first + "=" + it->second;
		_environment[i] = strdup(str.c_str());
	}
	return _environment;
}

void CGI::clean() {
	if (_init) {
		for (size_t i = 0; _environment[i]; i++) {
			if (_environment[i]) {
				free(_environment[i]);
				_environment[i] = nullptr;
			}
		}
		free(_environment);
		_environment = nullptr;
		free(_path);
		_path = nullptr;
	}
}

char **CGI::getEnvironment() const {return _environment;}

/**
 * Function execute cgi script with execve and forked process
 */
void	CGI::executeCGI() {

	int savedFd[2];
	FILE *file[2];
	int fd[2];
	pid_t pid;
	std::string newBody;

	if ((savedFd[IN] = dup(STDIN_FILENO)) == -1 || (savedFd[OUT] = dup(STDOUT_FILENO)) == -1)
		throw std::runtime_error(RED + std::string("Can't create file descriptor") + RESET);;
	if (!(file[IN] = tmpfile()) || !(file[OUT] = tmpfile()))
		throw std::runtime_error(RED + std::string("Can't create temporary file") + RESET);
	if ((fd[IN] = fileno(file[IN])) == -1 || (fd[OUT] = fileno(file[OUT])) == -1)
		throw std::runtime_error(RED + std::string("Can't create file descriptor") + RESET);

	write(fd[IN], _request->getBody().c_str(), _request->getBody().size());
	lseek(fd[IN], SEEK_SET, SEEK_SET);

	pid = fork();
	if (pid == -1) {
		_response->setCode(500);
		throw std::runtime_error(RED + std::string("Can't fork process") + RESET);
	}
	else if (pid == 0) {
		dup2(fd[IN], STDIN_FILENO);
		dup2(fd[OUT], STDOUT_FILENO);
		char * const * nlPointer = NULL;
		if (execve(_path, nlPointer, _environment) == -1)
			throw std::runtime_error(RED + std::string("Execve crashed!") + RESET);
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else
	{
		char buffer[CGI_BUFSIZE];
		waitpid(-1, NULL, 0);
		lseek(fd[OUT], SEEK_SET, SEEK_SET);
		ssize_t bytes = 1;
		while (bytes > 0) {
			bzero(buffer, CGI_BUFSIZE);
			bytes = read(fd[OUT], buffer, CGI_BUFSIZE);
			_bodySize += bytes;
			newBody += buffer;
		}
	}
	dup2(savedFd[IN], STDIN_FILENO);
	dup2(savedFd[OUT], STDOUT_FILENO);
	fclose(file[IN]);
	fclose(file[OUT]);
	close(fd[IN]);
	close(fd[OUT]);
	close(savedFd[IN]);
	close(savedFd[OUT]);
	if (pid == 0)
		exit(0);
	size_t pos;
	std::string cgiHeader;
	if ((pos = newBody.find(BODY_SEP, 0)) != std::string::npos) {
		cgiHeader = std::string(newBody, 0, pos + 4);
		newBody = std::string(newBody, pos + 4);
		if (cgiHeader.find("Status: ", 0) != std::string::npos)
			_response->setCode(std::atoi(cgiHeader.substr(8, 3).c_str()));
		if ((pos = cgiHeader.find("Content-Type: ", 0)) != std::string::npos)
			_request->setContentType(cgiHeader.substr(pos + 14, 24));
		_response->setCgiHeader(cgiHeader);
		_response->setBodySize((size_t)_bodySize - cgiHeader.size());
	}
    char *temp = (char *)malloc(sizeof(char) * newBody.size());
    for (size_t i = 0; i < newBody.size(); ++i) {
        temp[i] = newBody[i];
    }
	_response->setBody(temp);
}
