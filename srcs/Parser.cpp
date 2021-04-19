// CLion
// Created by Foster Grisella on 4/13/21 18:10.
//


#include <fstream>
#include "../includes/Parser.hpp"

Parser::~Parser() {

}

Parser::Parser(const Parser &other) {

}

Parser &Parser::operator=(const Parser &other) {
	return *this;
}

Parser::Parser(const std::string &fileName) {
	std::ifstream file(fileName);
	std::string   bigLine;
	if (!file.is_open()){
		throw std::runtime_error("file corrupted");
	}
	bigLine = this->readFile(file);
	file.close();
}


const std::vector<Server *> &Parser::getServers() const {
	return _servers;
}

std::string Parser::readFile(std::ifstream &fd) {
	std::string buf;
	std::stringstream o;
	std::string temp;

	int i = 0;


	while (std::getline(fd, buf)){
		if (buf == "server:") {
			if (!temp.empty()) {
				separateServers(temp, i);
				i++;
				o.clear();
			}
		}
		else if ((buf.find("server:") != std::string::npos && (buf.compare(0, 7, "server:")) && buf.size() != 7)) {
			throw std::runtime_error("config: server line error");
			//std::cout << "hello mfc " << std::endl;
		}
		o << buf << std::endl;
		temp = o.str();
	}
	if (temp.find("server:") != std::string::npos) {
		separateServers(temp, i);
	}
	return (o.str());
}

std::string getmyline(std::string& str, std::string com, int n){
	std::string temp;
	int i = 0;
	while (str[i] == '\t'){
		i++;
	}
	if (i != n){
		throw std::runtime_error("Tabulation error");
	}
	if (str[i + 1] && str.compare(i, com.size(), com)){
		throw std::runtime_error("Config file error");
	}
	temp = str.substr(i + com.size() + 1, str.size() - i);
	if (temp.find(com.c_str(), 0, com.size() - 1)){
		return (temp);
	}
	else{
		throw std::runtime_error("Config file error");
	}
}

int Parser::initLocations(std::vector<std::string> &strings, int count, int i){
	int j = 0;
	std::string temp;
	std::map<std::string, Location> locMap;
	while (count < strings.size()){
		if (strings[count].find("location:") != std::string::npos){
			temp = getmyline(strings[count],  "location:", 2);
			count++;
			Location *newLoc = new Location();
			while(strings[count].find("location:") == std::string::npos && count < strings.size()){
				if (strings[count] == "\n" || strings[count] == "")
					;
				else if (strings[count].find("root:") != std::string::npos){
					newLoc->setRoot(getmyline(strings[count], "root:", 3));
				}
				else if (strings[count].find("access-control-allow-methods:") != std::string::npos){
					newLoc->setAllowMethods(getmyline(strings[count], "access-control-allow-methods:", 3));
				}
				else if (strings[count].find("index:") != std::string::npos){
					newLoc->setRoot(getmyline(strings[count], "index:", 3));
				}
				else if (strings[count].find("cgi_path:") != std::string::npos){
					newLoc->setCgiPath(getmyline(strings[count], "cgi_path:",3));
				}
				else{
					throw std::runtime_error("Config parser error");
				}
				count++;
			}
			locMap[temp] = *newLoc;

		}
		else{
			std::cout << "Error: init location" << std::endl;
		}
	}
	_servers[i]->setLocation(locMap);
	return (count);
}

Server * Parser::separateServers(std::string &line, size_t i) {
	std::vector<std::string> strings;
	std::istringstream f(line);
	std::string s;

	Server *newServ = new Server();

	while (getline(f, s, '\n')) {
		strings.push_back(s);
	}
	int count = 1;
	while (strings[count].find("location:") == std::string::npos && count != strings.size()){
		if (strings[count] == "\n" || strings[count] == "")
			;
		else if (strings[count].find("host:") != std::string::npos){
			newServ->setHost(getmyline(strings[count], "host:", 1));
		}
		else if (strings[count].find("port:") != std::string::npos){
			newServ->setPort(getmyline(strings[count], "port:",1));
		}
		else if (strings[count].find("server_name:") != std::string::npos){
			newServ->setServerName(getmyline(strings[count], "server_name:", 1));
		}
		else if (strings[count].find("error_page:") != std::string::npos){
			newServ->setErrorPage(getmyline(strings[count], "error_page:", 1));
		}
		else{
			std::cout << "Error in the parser" << std::endl;
		}
		count++;
	}
	_servers.push_back(newServ);
	if (strings[count].find("location:")){
		count = initLocations(strings, count, i);
	} else{
		std::cout << "there is not locations" << std::endl;
	}
	if (count != strings.size())
		throw std::runtime_error("Config file error");
	return (_servers[i]);
}