// CLion
// Created by Foster Grisella on 4/13/21 16:24.
//


#include "../includes/Location.hpp"

Location::Location() {
	_autoIndex = false;
}

Location::~Location() {

}

void Location::setRoot(const std::string &root) {
	_root = root;
}

void Location::setAutoIndex(bool autoIndex) {
	_autoIndex = autoIndex;
}

void Location::setUrl(const std::string &url) {
	_url = url;
}

void Location::setAllowMethods(const std::string &allowMethods) {
	std::vector<std::string> newVec;
	if (allowMethods.find("GET") != std::string::npos)
		newVec.push_back("GET");
	if (allowMethods.find("PUT") != std::string::npos)
		newVec.push_back("PUT");
	if (allowMethods.find("POST") != std::string::npos)
		newVec.push_back("POST");
	if (allowMethods.find("HEAD") != std::string::npos)
		newVec.push_back("HEAD");
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
