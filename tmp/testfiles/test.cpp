// CLion
// Created by Foster Grisella on 4/18/21 21:20.
//


#include <iostream>
#include <unistd.h>
#include <vector>
class A{
public:
	A(int x = 0, const std::string &y = "qwe"){
		_x = x;
		_y = y;
	};
	int _x;
	std::string _y;
};

void test();

int main()
{
//	std::vector<A *> a;
//	for (int i = 0; i < 5; ++i) {
//		a.push_back(new A());
//	}
//	a.clear();
	test();
	std::cout << "lol" << std::endl;
	while (1);
}

void test() {
	std::vector<A *> a;
	for (int i = 0; i < 5; ++i) {
		a.push_back(new A());
	}
	for (int i = 0; i < 5; ++i) {
		delete a[i];
	}
	a.clear();
}
