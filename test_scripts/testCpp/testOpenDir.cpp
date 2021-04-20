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

class Index{
public:
	Index(const std::string &name, const std::string&dirRoot) : _name(name), _dirRoot(dirRoot){
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

void getDirectory();

int main()
{
//	testOpenDir();
//	testStat();
//	isDirCheck();
	getDirectory();
}

void getDirectory() {
	std::vector<Index> index;


	DIR *dir;
	struct dirent *dent;
	dir = opendir("/Users/fgrisell/CLionProjects/webserv");
	if(dir!=NULL)
	{
		while((dent=readdir(dir))!=NULL){
			index.push_back(Index(dent->d_name, "/Users/fgrisell/CLionProjects/webserv/"));
		}
	}
	closedir(dir);

	for (int i = 0; i < index.size(); ++i) {
		std::cout 	 << std::setw(20) << index[i]._name
					 << std::setw(20) << index[i]._size << "		"
					 << std::setw(20) << index[i]._lastMode;
	}
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
