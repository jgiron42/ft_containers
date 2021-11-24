#include <iostream>
#include "timer.hpp"
#include "is_integral.hpp"
#include "enable_if.hpp"
#include "map.hpp"
#include <map>
//#include "vector.hpp"
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


void test_tree1(ft::map<int, int> &m)
{
	m.insert(ft::make_pair<int, int>(4, 4));
	m.insert(ft::make_pair<int, int>(65, 65));
	m.insert(ft::make_pair<int, int>(23, 23));
	m.insert(ft::make_pair<int, int>(26, 26));
	m.insert(ft::make_pair<int, int>(86, 86));
	m.insert(ft::make_pair<int, int>(64, 64));
	m.insert(ft::make_pair<int, int>(95, 95));
	m.insert(ft::make_pair<int, int>(24, 24));
	m.insert(ft::make_pair<int, int>(32, 32));
	m.insert(ft::make_pair<int, int>(87, 87));
	m.insert(ft::make_pair<int, int>(25, 25));
	m.insert(ft::make_pair<int, int>(82, 82));
	m.insert(ft::make_pair<int, int>(95, 95));
	m.insert(ft::make_pair<int, int>(80, 80));
	m.insert(ft::make_pair<int, int>(69, 69));
	m.insert(ft::make_pair<int, int>(12, 12));
	m.insert(ft::make_pair<int, int>(93, 93));
	m.insert(ft::make_pair<int, int>(28, 28));
	m.insert(ft::make_pair<int, int>(66, 66));
	m.insert(ft::make_pair<int, int>(50, 50));
	m.insert(ft::make_pair<int, int>(38, 38));
	m.insert(ft::make_pair<int, int>(48, 48));
	m.insert(ft::make_pair<int, int>(92, 92));
	m.insert(ft::make_pair<int, int>(92, 92));
	m.insert(ft::make_pair<int, int>(64, 64));
	m.insert(ft::make_pair<int, int>(20, 20));
	m.insert(ft::make_pair<int, int>(15, 15));
	m.insert(ft::make_pair<int, int>(42, 42));
	m.insert(ft::make_pair<int, int>(3, 3));
	m.insert(ft::make_pair<int, int>(32, 32));
}

std::vector<int> test_tree2()
{
	std::vector<int> ret;
	ret.push_back(99);
	ret.push_back(59);
	ret.push_back(19);
	ret.push_back(32);
	ret.push_back(55);
	ret.push_back(8);
	ret.push_back(80);
	ret.push_back(66);
	ret.push_back(49);
	ret.push_back(89);
	ret.push_back(39);
	ret.push_back(83);
	ret.push_back(57);
	ret.push_back(98);
	ret.push_back(67);
	ret.push_back(35);
	ret.push_back(88);
	ret.push_back(45);
	ret.push_back(8);
	ret.push_back(97);
	ret.push_back(98);
	ret.push_back(38);
	ret.push_back(60);
	ret.push_back(40);
	ret.push_back(29);
	ret.push_back(22);
	ret.push_back(81);
	ret.push_back(90);
	ret.push_back(79);
	ret.push_back(83);
	return(ret);
}

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
		mft.insert(ft::make_pair<int, int>(tmp, tmp));
		mstd.insert(std::pair<int, int>(tmp, tmp));
		v.push_back(tmp);
		str1 += SSTR(tmp);
		if (i != 49)
			str1 += ", ";
//		std::cout << tmp << ", ";
	}
	str1+= "}";
	mft.rb_test();
	while (v.size() > 30)
	{
		tmp = v[std::rand() % v.size()];
		mft.erase(tmp);
		mstd.erase(tmp);
//		mft.rb_test();
		mft.print();
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

int main() {
	ft::map<int, int> m;
	int tmp;

	test_erase();
	return 0;

	struct timeval time;
	gettimeofday(&time, NULL);
	std::srand(time.tv_usec + (long int)1e6 * time.tv_sec);
	for (int i = 0; i < 50; i++)
	{
//		tmp = std::rand() % 100;
//		m.print();
//		m.insert(ft::make_pair<int, int>(tmp, tmp));
//		m.insert(ft::make_pair<int, int>(i, i));
//		std::cout << tmp << ", ";
	}

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
	std::cout << (is_equal ? "OK" : "KO") << std::endl;
}
