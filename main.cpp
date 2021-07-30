#include "vector.hpp"
#include <iostream>
#include "is_integral.hpp"
#include "enable_if.hpp"

int main()
{
	ft::vector<int> a;
	a.push_back(2);
	a.push_back(3);
	a.push_back(3);
	a.push_back(3);
	a.push_back(3);
	a.pop_back();
	ft::vector<int> b;
	b.push_back(2);
	b.push_back(3);
	b.push_back(3);
	for (int i = 0; i < (int)a.size(); i++)
		std::cout << a[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < (int)b.size(); i++)
		std::cout << b[i] << " ";
	std::cout << std::endl;
}
