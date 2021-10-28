#include <iostream>
#include "is_integral.hpp"
#include "enable_if.hpp"
#include "map.hpp"
#include "vector.hpp"
#include <vector>
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

//ft::vector<std::string> a;
//std::vector<int> b;
//
//for (int i = 0; i < 4; i++)
//	b.push_back(i);
//a.insert(a.begin(), 2, "zbeub");
}
