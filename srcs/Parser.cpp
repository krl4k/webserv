// CLion
// Created by Foster Grisella on 4/13/21 18:10.
//


#include "Parser.hpp"

Parser::~Parser() {

}

Parser::Parser(const Parser &other) {
	(void)other;
}

Parser &Parser::operator=(const Parser &other) {
	(void)other;
	return *this;
}

Parser::Parser(const std::string &fileName) {
	std::ifstream file(fileName);
	std::string bigLine;
	if (!file.is_open()) {
		throw std::runtime_error("file corrupted");
	}
	bigLine = this->readFile(file);
	file.close();

	//init whiteList
	for (size_t i = 0; i < _servers.size(); ++i) {
		try {
			if (!_servers[i]->getAuthBasicUserFile().empty())
				_servers[i]->setWhiteList(_servers[i]->getAuthBasicUserFile());
		} catch (std::exception &exception) {
			throw std::runtime_error(exception.what());
		}
	}
	for (size_t i = 0; i < _servers.size(); ++i){
	    for (size_t j = i + 1; j < _servers.size(); ++j){
	        if (_servers[i]->getPort() == _servers[j]->getPort())
                throw std::runtime_error("Port error");
	    }
	}

	for (size_t i = 0; i < _servers.size(); ++i) {
		if (_servers[i]->getServerName().empty())
			throw std::runtime_error("There is no server name");
		if (_servers[i]->getPort() < 1024)
			throw std::runtime_error("Port should be between 1024 and 65534");
		if (_servers[i]->getLocation().empty())
			throw std::runtime_error("There is no locations");
	}
}


const std::vector<Server *> &Parser::getServers() const {
	return _servers;
}

std::string Parser::readFile(std::ifstream &fd) {
	std::string buf;
	std::stringstream o;
	std::string temp;

	size_t i = 0;


	while (std::getline(fd, buf)) {
		if (buf == "server:") {
			if (!temp.empty()) {
				separateServers(temp, i);
				temp = "";
				i++;
				o.str("");
			}
		} else if ((buf.find("server:") != std::string::npos && (buf.compare(0, 7, "server:")) && buf.size() != 7)) {
			throw std::runtime_error("config: server line error");
		}
		o << buf << std::endl;
		temp = o.str();
	}
	if (temp.find("server:") != std::string::npos) {
		separateServers(temp, i);
	}
	if (o.str().empty())
		throw std::runtime_error("config: server line error");
	return (o.str());
}

std::string getmyline(std::string &str, std::string com, size_t n) {
	std::string temp;
	size_t i = 0;
	while (str[i] == '\t') {
		i++;
	}
	if (i != n) {
		throw std::runtime_error("Tabulation error");
	}
	if (str[i + 1] && str.compare(i, com.size(), com)) {
		throw std::runtime_error("Config file error");
	}
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '\0')
		throw std::runtime_error("Config file error");
	i = i + com.size();
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	temp = str.substr(i, str.size());
	if (str.find(com.c_str(), 0, com.size() - 1)) {
		return (temp);
	} else {
		throw std::runtime_error("Config file error");
	}
}

size_t Parser::initLocations(std::vector<std::string> &strings, size_t count, size_t i) {
	std::string temp;
	std::map<std::string, Location> locMap;

	while (count < strings.size()) {
		if (strings[count].find("location:") != std::string::npos) {
			temp = getmyline(strings[count], "location:", 2);
			count++;
			Location *newLoc = new Location();
			while (count < strings.size() && strings[count].find("location:") == std::string::npos) {
				if (strings[count] == "\n" || strings[count] == "" || strings[count][0] == '#');
				else if (strings[count].find("root:") != std::string::npos) {
					newLoc->setRoot(getmyline(strings[count], "root:", 3));
				} else if (strings[count].find("access-control-allow-methods:") != std::string::npos) {
					newLoc->setAllowMethods(getmyline(strings[count], "access-control-allow-methods:", 3));
				} else if (strings[count].find("autoindex:") != std::string::npos) {
					newLoc->setAutoIndex(getmyline(strings[count], "autoindex:", 3));
				} else if (strings[count].find("index:") != std::string::npos) {
					newLoc->setIndex(getmyline(strings[count], "index:", 3));
				} else if (strings[count].find("cgi_path:") != std::string::npos) {
					newLoc->setCgiPath(getmyline(strings[count], "cgi_path:", 3));
				} else if (strings[count].find("client_max_body_size:") != std::string::npos) {
					newLoc->setClientMaxBodySize(getmyline(strings[count], "client_max_body_size:", 3));
				} else {
					throw std::runtime_error("Config parser error");
				}
				count++;
			}
			locMap[temp] = *newLoc;
			delete newLoc;
		} else {
			std::cout << "Error: init location" << std::endl;
		}
	}
	_servers[i]->setLocation(locMap);
	return (count);
}

Server *Parser::separateServers(std::string &line, size_t i) {
	std::vector<std::string> strings;
	std::istringstream f(line);
	std::string s;

	Server *newServ = new Server();

	while (getline(f, s, '\n')) {
		s[s.size()] = '\0';
		strings.push_back(s);
	}
	size_t count = 1;
	while (strings[count].find("location:") == std::string::npos && count != strings.size()) {
		if (strings[count] == "\n" || strings[count] == "" || strings[count][0] == '#');
		else if (strings[count].find("host:") != std::string::npos) {
			newServ->setHost(getmyline(strings[count], "host:", 1));
		} else if (strings[count].find("port:") != std::string::npos) {
			newServ->setPort(getmyline(strings[count], "port:", 1));
		} else if (strings[count].find("server_name:") != std::string::npos) {
			newServ->setServerName(getmyline(strings[count], "server_name:", 1));
		} else if (strings[count].find("error_page:") != std::string::npos) {
			newServ->setErrorPage(getmyline(strings[count], "error_page:", 1));
		} else if (strings[count].find("auth_basic_user_file:") != std::string::npos) {
			newServ->setAuthBasicUserFile(getmyline(strings[count], "auth_basic_user_file:", 1));}
		else {
			std::cout << "Error in the parser" << std::endl;
		}
		count++;
	}
	_servers.push_back(newServ);
	if (strings[count].find("location:")) {
		count = initLocations(strings, count, i);
	} else {
		std::cout << "there is not locations" << std::endl;
	}
	if (count != strings.size())
		throw std::runtime_error("Config file error");
	return (_servers[i]);
}