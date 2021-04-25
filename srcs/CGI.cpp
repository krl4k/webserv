#include "../includes/CGI.hpp"

CGI::CGI(Server *server, Client *client, const char *path) {
	_request = client->getRequest();
	_response = client->getResponse();
	_path = strdup(path);
	setEnvironment(server, client);
	executeCGI();
}

CGI::~CGI() {
//	for (int i = 0; i < sizeof(_environment); i++)
//		free(_environment[i]);
//	free(_environment);
//	//for (int i = 0; i < sizeof(_arguments); i++)
//		free(_arguments[i]);
//	free(_arguments);
//	free(_path);
}

/**
 * Check http://www6.uniovi.es/~antonio/ncsa_httpd/cgi/env.html for environments.
 */
void CGI::setEnvironment(Server *server, Client *client) {
	std::map<std::string, std::string> env;

	env["AUTH_TYPE="] = "basic";								 //use to check user
	env["CONTENT_LENGTH="] = std::to_string(_request->getBody().size());// - The length of the said
														 // content as given by the client.
	env["CONTENT_TYPE="] = _request->getContentType();	 // - POST, GET, PUT
	env["GATEWAY_INTERFACE="] = "CGI/1.1";
	env["PATH_INFO="] = _request->getPath();				 // - The extra path information, as given by the client
														 //This information should be decoded by the server if it comes from a URL
	env["PATH_TRANSLATED="] = _request->getPath();						 // - The server provides a translated version of PATH_INFO,
														 // which takes the path and does any virtual-to-physical mapping to it
	env["QUERY_STRING="] = _request->getPath() + "?" + _request->getQueryString();			 // - The information which follows the ? in the URL,
											   			 // It should not be decoded in any fashion
	env["REMOTE_ADDR="] = server->getHost();			 // - The IP address of the remote host making the request
	env["REMOTE_IDENT="] = "basic";							 // - name for remote user retrieved by server
														 // (server need supports RFC 931)
	env["REMOTE_USER="] = "basic";							 // - if server supports auth(login)
	env["REQUEST_METHOD="] = _request->getMethod();		 // - The method with which the request was made.
	env["REQUEST_URI="] = _request->getPath();
	env["SCRIPT_NAME="] = _request->getPath();			 // - A virtual path to the script being executed,
														 // used for self-referencing URLs
	env["SERVER_NAME="] = server->getServerName();
	env["SERVER_PORT="] = std::to_string(server->getPort());			 // - The port number to which the request was sent.
	env["SERVER_PROTOCOL="] = "HTTP/1.1";				 // - The name and revision of the information protocol
														 // this request came in with.
	env["SERVER_SOFTWARE="] = "KiRoTaMagic/6.9";
	setEnvToString(env);
	env.clear();
}

/**
 * set path in arguments
 */
void CGI::setArguments() {
	_arguments = (char **)calloc(3, sizeof(char *));
	_arguments[0] = strdup(_path);
	_arguments[1] = strdup(_path);
}

/**
 * rotate env from map to string for execve
 * @param std::map<>env
 * @return string of env
 */
char **CGI::setEnvToString(std::map<std::string, std::string> env) {
	_environment = (char **)calloc(env.size(), sizeof(char *));

	std::map<std::string, std::string>::iterator it;
	int i = 0;
	for (it = env.begin(); it != env.end(); it++, i++) {
		std::string str = it->first + it->second;
		_environment[i] = strdup(str.c_str());
	}
	return _environment;
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
		if (execve(_path, _arguments, _environment) == -1)
			throw std::runtime_error(RED + std::string("Execve crashed!") + RESET);
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else {
		char buffer;
		waitpid(-1, NULL, 0);
		lseek(fd[OUT], SEEK_SET, SEEK_SET);
		while (read(fd[OUT], &buffer, 1) > 0)
			newBody += buffer;
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
	_response->setBody(newBody);
}