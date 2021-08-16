#include <iostream>
#include "is_integral.hpp"
#include "enable_if.hpp"
#include "map.hpp"
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <sys/time.h>

int main()
{
	// ================================= VECTOR =================================
//	ft::vector<int> a;
//	a.push_back(2);
//	a.push_back(3);
//	a.push_back(3);
//	a.push_back(3);
//	a.push_back(3);
//	a.pop_back();
//	ft::vector<int> b;
//	b.push_back(2);
//	b.push_back(3);
//	b.push_back(3);
//	for (int i = 0; i < (int)a.size(); i++)
//		std::cout << a[i] << " ";
//	std::cout << std::endl;
//	for (int i = 0; i < (int)b.size(); i++)
//		std::cout << b[i] << " ";
//	std::cout << std::endl;

	// ================================= MAP =================================
	ft::map<int, int> m;
	int tmp;

	struct timeval time;
	gettimeofday(&time, NULL);
	std::srand(time.tv_usec + (long int)1e6 * time.tv_sec);
	for (int i = 0; i < 500; i++)
	{
		tmp = std::rand() % 1000;
//		m.print();
		m.insert(ft::make_pair<int, int>(tmp, tmp));
	}
	std::cout << DEFAULT_OUTPUT;
	m.print();
//	std::cout << m.at(123) << std::endl;
//	m.at(123) = 45;
//	std::cout << m.at(123) << std::endl;
//	m.insert(ft::make_pair<int, int>(17, 17));
//	m.print();
//	m.insert(ft::make_pair<int, int>(24, 24));
//	m.print();
//	m.insert(ft::make_pair<int, int>(14, 14));
//	m.print();
//	m.insert(ft::make_pair<int, int>(70, 70));
//	m.print();
//	m.insert(ft::make_pair<int, int>(27, 27));
//	m.print();
//	m.insert(ft::make_pair<int, int>(66, 66));
//	m.print();
//	m.insert(ft::make_pair<int, int>(72, 72));
//	m.print();
//	m.test_rotate_left();
//	m.print();
}
