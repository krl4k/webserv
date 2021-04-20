// CLion
// Created by Foster Grisella on 4/20/21 18:41.
//

#include <ios>
#include <dirent.h>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <iomanip>
#include <unistd.h>
#include <sstream>
#include <ios>
#include <fstream>

class DirectoryContent{
public:
	DirectoryContent(const std::string &name, const std::string&dirRoot) : _name(name), _dirRoot(dirRoot){
		struct stat structstat;

		std::string path(_dirRoot + name);
		if (stat(path.c_str(), &structstat) < 0)
		{
			std::cout << strerror(errno) << std::endl;
			return;
		}

		time_t time = (time_t)structstat.st_mtimespec.tv_sec;
		_lastMode = ctime(&time);
		if (S_ISDIR(structstat.st_mode)){
			_size = -1;
//			_size = (structstat.st_size);
		}
		if (S_ISREG(structstat.st_mode)){
			_size = (structstat.st_size);
		}

	}

	std::string _name;
	std::string	_lastMode;
	off_t _size;
	std::string _dirRoot;
};

void testOpenDir();

void testStat();

void isDirCheck();

std::vector<DirectoryContent> getDirectory() {
	std::vector<DirectoryContent> directoryContent;


	DIR *dir;
	struct dirent *dent;
	dir = opendir("/Users/fgrisell/CLionProjects/webserv");
	if(dir!=NULL)
	{
		while((dent=readdir(dir))!=NULL){
			directoryContent.push_back(DirectoryContent(dent->d_name, "/Users/fgrisell/CLionProjects/webserv/"));
		}
	}
	closedir(dir);

	for (int i = 0; i < directoryContent.size(); ++i) {
		std::cout << std::setw(20) << directoryContent[i]._name
				  << std::setw(20) << directoryContent[i]._size << "		"
				  << std::setw(20) << directoryContent[i]._lastMode;
	}
	return directoryContent;
}

std::stringstream genPart(std::vector<DirectoryContent> dirCont){
	std::stringstream str;

	str <<"<tr>";
	str <<"<td height=\"20px\">";
	str <<"<p>";
	str <<"<a href=\"";
	str <<"https://github.com/\">";
	str <<"<img src=\"/Users/fgrisell/CLionProjects/webserv/html/autoIndexPage/icons/back.png\" width=\"15px\" height=\"15px\">";
	str <<"</a>";
	str <<"</p>";
	str <<"</td>";
	str <<"</t";

	for (int i = 0; i < dirCont.size(); ++i) {
		if (dirCont[i]._name[0] == '.')
			continue;
		str << 	"<tr>";
		str << 	"<td height=\"20px\">";
		str << 	"<img src=\"";
		str << 	"autoIndexPage/autoIndex/folder1.jpeg";
		str << 	"\" ";
		str << 	"width=\"15px\" height=\"15px\">";
		str << 	"<a href=\"https://github.com/\">";
//		if (dirCont[i]._name[0] != '.')
			str <<  dirCont[i]._name;
		str <<  "</a>";
		str << 	"</td>";
		str << 	"<td>";
		str <<	"<a>";
		if (dirCont[i]._size != -1)
			str << 	dirCont[i]._size;
	 	str <<	"</a>";
	 	str <<	"</td>";
		str << 	"<td>";
		str << 	dirCont[i]._lastMode;
		str <<  "</td>";
		str << "</tr>";
	}
	return str;
}

int main()
{
//	testOpenDir();
//	testStat();
//	isDirCheck();
	std::vector<DirectoryContent> directoryContent = getDirectory();
	std::stringstream str;
	str << "<!DOCTYPE html>"
		   "<html lang=\"en\">"
		   "<head>"
		   "    <meta charset=\"UTF-8\">"
		   "    <title>DirectoryContent of _PATH_</title>"
		   "</head>"
		   "<body>"
		   "<center>"
		   "    <div>"
		   "        <table>"
		   "            <!--        title-->"
		   "            <tr>"
		   "                <th height=\"20px\" >Name</th>"
		   "                <th height=\"20px\">Size</th>"
		   "                <th height=\"20px\">Date</th>"
		   "            </tr>"
		   "            <!--        back button-->"
		   "            <tr>"
		   "                <td height=\"20px\">"
		   "                    <p>"
		   "                        <a href=\"https://github.com/\">"
		   "                            <img src=\"icons/back.png\" width=\"15px\" height=\"15px\">"
		   "                        </a>"
		   "                    </p>"
		   "                </td>"
		   "            </tr>"
		   "            <!--        directory content-->";



		 	str << genPart(directoryContent).str();

		 	str << "   </table>"
				   "    </div>"
				   ""
				   "</center>"
				   "</body>"
				   "</html>";

		 	std::ofstream file("/Users/fgrisell/CLionProjects/webserv/test_scripts/testCpp/test.html");

		 	file << str.str();

		 	file.close();
}


void isDirCheck() {
//	if (open("/Users/fgrisell/CLionProjects/webserv/", O_RDONLY) < 0){
//		std::cout << "is not file!" << std::endl;
//		return;
//	}
	struct stat structstat;
	int r = stat("/Users/fgrisell/CLionProjects/webserv", &structstat);

	if (S_ISDIR(structstat.st_mode)){
		std::cout << "is directory!" << std::endl;
	}
	if (S_ISREG(structstat.st_mode)){
		std::cout << "is file!" << std::endl;
	}
	if (S_ISSOCK(structstat.st_mode)){
		std::cout << "is sock!" << std::endl;
	}

}


void testStat() {
	struct stat structstat;
	int r = stat("/Users/fgrisell/CLionProjects/webserv", &structstat);

	std::cout << "webserv last mod = " << structstat.st_mtimespec.tv_sec << std::endl;
	std::cout << "webserv size = " << structstat.st_size << "B" << std::endl;

	std::string date;
	time_t time = (time_t)structstat.st_mtimespec.tv_sec;
	date = ctime(&time);

	std::cout << "data = " << date << std::endl;
}

void testOpenDir() {
	DIR *dir;
	struct dirent *dent;
	dir = opendir("/Users/fgrisell/CLionProjects/webserv");
	if(dir!=NULL)
	{
		while((dent=readdir(dir))!=NULL)
			std::cout << "name = " << dent->d_name << ", ino = " << dent->d_reclen << std::endl;
	}
	closedir(dir);
}
