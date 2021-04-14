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

private:
	std::string					_root;
	bool						_autoIndex;
	std::string					_url;
	std::vector<std::string>	_allowMethods;
	//cgi
	//limit

};

#endif //WEBSERV_LOCATION_HPP
