#include "CGI.hpp"

CGI::CGI(Client *client, char *path)

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

void CGI::makeProccess() {

}

void CGI::executeCGI(HTTPResponse &response) {

	int		fd;
	size_t	size;
	pid_t	pid;
	int 	pipes[2];

	if ((pipe(pipes)) != 0)
		throw std::runtime_error(RED + std::string("Error in pipe") + ENDC);
	size = 0;
	if (!(fd = open("file", O_CREATE | O_RDWR | O_TRUNC, 0644)) < 0)
		throw std::runtime_error(RED + std::string("Can't open file") + ENDC);
	if ((pid = fork()) == 0) {
		close(pipes[1]);
		dup2(pipes[0], 0);
		close(pipes[0]);
		dup2(fd, 1);
		close(fd);
		exit(execve(_arguments[0], _arguments, getEnvironment()));
	}
	else if (pid == -1)
		throw std::runtime_error(RED + std::string("Fork failure") + ENDC);
	else {
		
	}
}