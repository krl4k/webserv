// CLion
// Created by Foster Grisella on 4/13/21 16:24.
//


#ifndef WEBSERV_LOCATION_HPP
#define WEBSERV_LOCATION_HPP

#include <string>
#include <iostream>
#include <vector>


class Location {
public:

	Location();

	virtual ~Location();

//	Location(const Location &other);
//
//	Location &operator=(const Location &other);

	void setRoot(const std::string & root);

	bool getAutoIndex() const { return _autoIndex;}

	void setAutoIndex(std::string autoIndex);

	void setUrl(const std::string & url);

	void setAllowMethods(const std::string & allowMethods);

	const std::string &getRoot() const;

	bool isAutoIndex() const;

	const std::string & getUrl() const;

	const std::vector<std::string> & getAllowMethods() const;

private:
	std::string					_root;
	bool						_autoIndex = true;
	std::string					_url;
	std::vector<std::string>	_allowMethods;
	std::string 				_cgiPath;
	size_t 						_clientMaxBodySize;
	std::string 				_index;
public:
	const std::string & getIndex() const;

	void setIndex(const std::string &index);

public:
	size_t getClientMaxBodySize() const;

	void setClientMaxBodySize(std::string clientMaxBodySize);

public:
	const std::string &getCgiPath() const;

	void setCgiPath(const std::string &cgiPath);
	//cgi
	//limit

};

#endif //WEBSERV_LOCATION_HPP
