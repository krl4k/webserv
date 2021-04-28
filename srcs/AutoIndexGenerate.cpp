// CLion
// Created by Foster Grisella on 4/23/21 13:29.
//

#include "HttpResponse.hpp"



class DirectoryContent{
public:
	DirectoryContent(const std::string &name, const std::string &dirRoot) : _name(name), _dirRoot(dirRoot){
		struct stat structstat;

		std::cout << "dir root = " << dirRoot << name << std::endl;
		std::string path(_dirRoot + name);
		if (stat(path.c_str(), &structstat) < 0)
		{
			return;
		}
		time_t time = GET_TIME_OF_STRUCT(structstat);
		_lastMode = ctime(&time);

		if (S_ISDIR(structstat.st_mode))
			_size = -1;
		if (S_ISREG(structstat.st_mode))
			_size = (structstat.st_size);
	}

	const std::string &getName() const {
		return _name;
	}

	const std::string &getLastMode() const {
		return _lastMode;
	}

	off_t getSize() const {
		return _size;
	}

	const std::string &getDirRoot() const {
		return _dirRoot;
	}

private:
	std::string _name;
	std::string	_lastMode;
	off_t		_size;
	std::string _dirRoot;
};


static std::vector<DirectoryContent> getDirectory(const std::string &path) {
	std::vector<DirectoryContent> directoryContent;

	DIR *dir;
	struct dirent *dent;

//	const char *path_c_str = path.c_str();

	dir = opendir(path.c_str());
	if(dir!=NULL)
	{
		while((dent=readdir(dir))!=NULL){
			directoryContent.push_back(DirectoryContent(dent->d_name, path));
		}
		closedir(dir);
	}

	for (size_t i = 0; i < directoryContent.size(); ++i) {
		std::cout << std::setw(20) << directoryContent[i].getName()
				  << std::setw(20) << directoryContent[i].getSize() << "		"
				  << std::setw(20) << directoryContent[i].getLastMode();
	}
	return directoryContent;
}

void generateTitle(const std::string &path, std::stringstream &str){
	//title--------------------------------
	str << 	"<!DOCTYPE html>"
		   	"<html lang=\"en\">"
		   	"<head>"
		   	"<meta charset=\"UTF-8\">"
	 		"<title>"
			"DirectoryContent of ";
	//add path
	str <<	path;
	str << "</title>";
	//--------------------------------------

	//----header----------------------------
	str <<	"</head>"
		   	"<body>"
		   	"<center>"
		   	"<div>"
		   	"<table>"
		   	"<tr>"
		   	"<th height=\"20px\" >Name</th>"
		   	"<th height=\"20px\">Size</th>"
		   	"<th height=\"20px\">Date</th>"
		   	"</tr>";
	//--------------------------------------
}

void generateBackButton(const std::string &path, std::stringstream &str) {
	str <<"<tr>";
	str <<"<td height=\"20px\">";
	str <<"<p>";

	//add reference------
	str <<	"<a href=\"";

	//todo check and rewrite
	std::string back(path, 0, path.rfind('/'));
	back = std::string(back, 0, back.rfind('/') + 1);
	str <<	back;
	str <<	"\">";
	//------------------

	str << "<img src=\"../../html/autoIndexPage/icons/back.png\" width=\"15px\" height=\"15px\">";
	str <<"</a>";
	str <<"</p>";
	str <<"</td>";
	str <<"</tr>";
}

void generateAll(const std::string &path, std::stringstream &str, std::vector<DirectoryContent> dirCont) {
	for (size_t i = 0; i < dirCont.size(); ++i) {
		if (dirCont[i].getName()[0] == '.')
			continue;
		str << 	"<tr>";
		str << 	"<td height=\"20px\">";
		str << 	"<img src=\"";
		str << 	"autoIndexPage/autoIndex/folder1.jpeg";
		str << 	"\" ";
		str << 	"width=\"15px\" height=\"15px\">";

		//path-----------------------------------
		str << 	"<a href=\"";

		str << path + dirCont[i].getName();


//		char actualPath[123];
//		std::cout << "paath = " << path << std::endl;
//		char *rP = realpath(path.c_str(), actualPath);
		/*std::cout << "real path = " << rP << std::endl;
		std::cout << "paath = " << path << std::endl;
		str << std::string(rP) + "/" + dirCont[i].getName();
		*/

		str << "\">";

		//---------------------------------------
		if (dirCont[i].getSize() != -1)
			str << "<img src=\"../../html/autoIndexPage/icons/folder.png\" width=\"15px\" height=\"15px\">";
		else
			str << "<img src=\"../../html/autoIndexPage/icons/file.png\" width=\"15px\" height=\"15px\">";
		str <<  dirCont[i].getName();
		str <<  "</a>";
		str << 	"</td>";
		str << 	"<td>";
		str <<	"<a>";
		if (dirCont[i].getSize() != -1)
			str << 	dirCont[i].getSize() << " B";
		str <<	"</a>";
		str <<	"</td>";
		str << 	"<td>";
		str << 	dirCont[i].getLastMode();
		str <<  "</td>";
		str << "</tr>";
	}
}

void generateClose(std::stringstream &str) {
	str << "</table>"
		   "</div>"
		   "</center>"
		   "</body>"
		   "</html>";
}

std::string HttpResponse::getAutoIndexPage(std::string &path) {
	std::cout << "path = " << path << std::endl;

	std::vector<DirectoryContent> directoryContent = getDirectory(path);

	std::stringstream str;

	generateTitle(path, str);
	generateBackButton(path, str);
	generateAll(path, str, directoryContent);
	generateClose(str);

//	return str.str();
	return std::string(str.str());
}
/*

int main(){
//	std::string path = "/Users/fgrisell/CLionProjects/webserv/srcs";
	std::string path = "../srcs";

	if (path.back() != '/')
		path.append("/");
	std::cout << "path = " << path << std::endl;

	std::vector<DirectoryContent> directoryContent = getDirectory(path);

	std::string autoIndexPage;
	std::stringstream str;

	generateTitle(path, str);
	generateBackButton(path, str);
	generateAll(path, str, directoryContent);
	generateClose(path, str);

//	autoIndexPage = str.str();

	std::ofstream file("/Users/fgrisell/CLionProjects/webserv/test_scripts/testCpp/test.html");

	file << str.str();

	file.close();
}
*/
