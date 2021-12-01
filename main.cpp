#include <iostream>
#include "timer.hpp"
#include "is_integral.hpp"
#include "enable_if.hpp"
#include "map.hpp"
#include <map>
#include "vector.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <sys/time.h>
#include <list>
#include <utility>
//#include "shitty_allocator.hpp"
#include <streambuf>
#include <strstream>
#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

//#include "basic_allocator.hpp"

std::map<int,int> fill_std(std::vector<int> v)
{
	std::map<int,int> ret;
	for (auto i : v)
		ret.insert(std::make_pair(i, i));
	return (ret);
}

ft::map<int,int> fill_ft(std::vector<int> v)
{
	ft::map<int,int> ret;
	for (auto i : v)
		ret.insert(ft::make_pair(i, i));
	return (ret);
}

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
void	test_vector();

template <class T>
void test_ninsert(int n, T &v)
{
	timer<true> t;
	while (--n >= 0)
		v.insert(v.begin(), n);
}


template <class T>
void test_nerase(int n, T &v)
{
	timer<true> t;
	while (--n >= 0)
		v.erase(v.begin() + std::rand() % v.size());
}

class foo {
public:

	foo() {
		std::cout << this << std::endl;
	}
	foo(foo const &) {
		std::cout << "copy " << this << std::endl;
	}
	foo &operator=(foo const &)
	{
		std::cout << "assign " << this << std::endl;
		return (*this);
	}
};

int main() {
	ft::map<int, int> m;
	int tmp;

//		test_vector();

	/*
//	test_erase();
//	return 0;
	std::vector<int> vstd( );
	ft::vector<int> vft( );
//	foo a = foo();
//	std::vector<foo> vfoo;
//	vfoo.insert(vfoo.begin(), foo());
//	vfoo.insert(vfoo.begin(), foo());
//	vfoo.insert(vfoo.begin(), foo());
//	for (int n = 10000; n > 1000; n /= 10)
	for (int n = 1; n <= 10000; n *= 10)
	{
//		std::srand(1);
		std::cout << "std " << n << std::endl;
		test_ninsert(n, vstd);
//		test_nerase(n, vstd);
		std::cout << " us" <<  std::endl;
//		std::srand(1);
		std::cout << "ft " << n << std::endl;
		test_ninsert(n, vft);
//		test_nerase(n, vft);
		std::cout << " us" <<  std::endl;

	}
*/

	struct timeval time;
	gettimeofday(&time, NULL);
	std::srand(time.tv_usec + (long int)1e6 * time.tv_sec);
	for (int i = 0; i < 50; i++)
	{
		tmp = std::rand() % 100;
//		m.print();
		m.insert(ft::make_pair<int, int>(tmp, tmp));
//		m.insert(ft::make_pair<int, int>(i, i));
//		std::cout << tmp << ", ";
	}
	m.print();
/*
	std::map<int, int> mstd = fill_std(std::vector<int>({10, 65, 31, 39, 56, 55, 10, 8, 20, 52, 3, 74, 84, 88, 2, 76, 2, 61, 16, 5, 64, 47, 83, 94, 18, 59, 61, 90, 88, 96, 3, 99, 13, 35, 90, 21, 42, 1, 29, 62, 5, 33, 89, 41, 21, 91, 70, 75, 52, 86}));
	m = fill_ft(std::vector<int>({10, 65, 31, 39, 56, 55, 10, 8, 20, 52, 3, 74, 84, 88, 2, 76, 2, 61, 16, 5, 64, 47, 83, 94, 18, 59, 61, 90, 88, 96, 3, 99, 13, 35, 90, 21, 42, 1, 29, 62, 5, 33, 89, 41, 21, 91, 70, 75, 52, 86}));
//	m = fill_ft(test_tree2());
	m.print();
	std::cout << std::endl;


	m.erase(41);
	mstd.erase(41);
	m.erase(47);
	mstd.erase(47);
	m.print();
	m.erase(42);
	mstd.erase(42);
	m.print();


	bool is_equal = true;
	auto i1 = m.begin();
	auto i2 = mstd.begin();
	while (i1 != m.end() && i2 != mstd.end())
	{
		is_equal &= (i1->first == i2->first);
		++i1;
		++i2;
	}
//	i1 = m.begin();
//	i2 = mstd.begin();
//	while (i1 != m.end())
//		std::cout << i1++->first << " ";
//	std::cout << std::endl;
//	while (i2 != mstd.end())
//		std::cout << i2++->first << " ";
//	std::cout << std::endl;
	is_equal &= (i1 == m.end() && i2 == mstd.end());
	std::cout << (is_equal ? "OK" : "KO") << std::endl;*/
}
