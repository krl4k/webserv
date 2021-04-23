// CLion
// Created by Foster Grisella on 4/13/21 16:24.
//

#include "../includes/Location.hpp"
#include <string>
#include <sstream>

Location::Location() {
	_autoIndex = true;
	_clientMaxBodySize = 0;
}

Location::~Location() {
}

void Location::setRoot(const std::string &root) {
	_root = root;
}

void Location::setAutoIndex(std::string autoIndex) {

	if (autoIndex == "on"){	_autoIndex = true;	}
	else if (autoIndex == "off"){ _autoIndex = false;}
	else{
		throw std::runtime_error("autoindex error in config file");
	}
}

void Location::setUrl(const std::string &url) {
	_url = url;
}

void Location::setAllowMethods(const std::string &allowMethods) {
	std::vector<std::string> strings;
	std::string temp = allowMethods;
	std::string s;
	std::replace(temp.begin(), temp.end(), '\t', ' ');
	std::istringstream f(temp);

	while (getline(f, s, ' ')) {
		s[s.size()] = '\0';
		strings.push_back(s);
	}
	std::vector<std::string> newVec;
	for (size_t i = 0; i < strings.size(); ++i) {
		if (!strings[i].compare("GET"))
			newVec.push_back("GET");
		else if (!strings[i].compare("PUT"))
			newVec.push_back("PUT");
		else if (!strings[i].compare("POST"))
			newVec.push_back("POST");
		else if (!strings[i].compare("HEAD"))
			newVec.push_back("HEAD");
		else if (!strings[i].compare("") || !strings[i].compare(" "))
			;
		else
			throw std::runtime_error("Allow methods error");
	}
	if (newVec.empty())
		throw std::runtime_error("Allow methods error");
	for (size_t i = 0; i < newVec.size(); ++i){
		for (size_t j = i + 1; j < newVec.size(); ++j)
			if (newVec[i] == newVec[j] && i != j)
				throw std::runtime_error("Allow methods error");
	}
	_allowMethods = newVec;
}

const std::string &Location::getRoot() const {
	return _root;
}

bool Location::isAutoIndex() const {
	return _autoIndex;
}

const std::string &Location::getUrl() const {
	return _url;
}

const std::vector<std::string> &Location::getAllowMethods() const {
	return _allowMethods;
}

const std::string &Location::getCgiPath() const {
	return _cgiPath;
}

void Location::setCgiPath(const std::string &cgiPath) {
	_cgiPath = cgiPath;
}

size_t Location::getClientMaxBodySize() const
{
	return _clientMaxBodySize;
}

void Location::setClientMaxBodySize(std:: string clientMaxBodySize)
{

	try{
		_clientMaxBodySize = std::stoi(clientMaxBodySize);
	}
	catch (std::exception &e){
		throw std::runtime_error("setClientMaxBodySize error");
	}
}

const std::string &Location::getIndex() const {
	return _index;
}

void Location::setIndex(const std::string &index) {
	_index = index;
}