#define DEBUG
#include "srcs/containers/map.hpp"
#include "srcs/containers/vector.hpp"
#include "srcs/containers/stack.hpp"
#include <string>
#include "srcs/utils/pair.hpp"
#include <map>
#include <vector>
#include <stack>
#include <unistd.h>
#include <sys/time.h>
#ifndef NAMESPACE
# define NAMESPACE ft
#endif


int	test_erase()
{
	ft::map<int,int> mft;
	std::map<int,int> mstd;
	std::vector<int>	v;
	std::string str1 = "{";
	std::string str2 = "{";
	int tmp;

	struct timeval time;
	gettimeofday(&time, NULL);
	std::srand(time.tv_usec + (long int)1e6 * time.tv_sec);
	for (int i = 0; i < 50; i++)
	{
		tmp = std::rand() % 100;
		mft.insert(ft::make_pair<int, int>(i, i));
//		mft.insert(ft::make_pair<int, int>(tmp, tmp));
		mstd.insert(std::pair<int, int>(i, i));
//		mstd.insert(std::pair<int, int>(tmp, tmp));
		v.push_back(i);
//		v.push_back(tmp);
		str1 += SSTR(tmp);
		if (i != 49)
			str1 += ", ";
//		std::cout << tmp << ", ";
	}
	str1+= "}";
	while (mft.print() && v.size() > 45)
	{
		tmp = v[std::rand() % v.size()];
		mft.erase(tmp);
		mstd.erase(tmp);
		str2 += SSTR(tmp);
		if (v.size() > 1)
			str2 += ", ";

		bool is_equal = true;
		auto i1 = mft.begin();
		auto i2 = mstd.begin();
		while (i1 != mft.end() && i2 != mstd.end())
		{
			is_equal &= (i1->first == i2->first);
			++i1;
			++i2;
		}
		is_equal &= (i1 == mft.end() && i2 == mstd.end());
		std::cout << (is_equal ? "OK" : "KO") << std::endl;
	}
	str2 += "}";

}
int main()
{
	std::map<int, int> m;
	m.erase(2);
	test_erase();
}