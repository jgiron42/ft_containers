
#include "vector.hpp"
#include <vector>
#include <set>
#include <cstdlib>
#include <string>

#ifndef NAMESPACE
#define NAMESPACE ft
#endif
#ifndef TYPE
#define TYPE std::string
#endif
#ifndef DEFAULTVAL
TYPE default_val[] = {"zbeub", "", "zbib", "coucou", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};
#endif

TYPE	get_value()
{
	return (TYPE(default_val[std::rand() % (sizeof(default_val) / sizeof(TYPE))]));
}

void	print_full_vect(const NAMESPACE::vector<TYPE> &v)
{
	std::cout << "empty: " << v.empty() << std::endl;
	std::cout << "size: " << v.size() << std::endl;
	std::cout << "capacity: " << v.capacity() << std::endl;
	std::cout << "max_size: " << v.max_size() << std::endl;
	if (!v.empty()) {
		std::cout << "front: " << v.front() << std::endl;
		std::cout << "back: " << v.back() << std::endl;
		std::cout << "*data (" << v.data() << "):" << std::endl;
		std::cout << *v.data() << std::endl;
	}
	std::cout << "content:" << std::endl;
	for (NAMESPACE::vector<TYPE>::const_iterator i = v.begin(); i != v.end(); i++)
		std::cout << " " << *i << std::endl;
	std::cout << "reverse content:" << std::endl;
	for (NAMESPACE::vector<TYPE>::const_reverse_iterator i = v.rbegin(); i != v.rend(); i++)
		std::cout << " " << *i << std::endl;
}

void	test_clear(NAMESPACE::vector<TYPE> &v1, NAMESPACE::vector<TYPE> &v2)
{
	std::cout << "test_clear:" << std::endl;
	NAMESPACE::vector<TYPE> v3(v1);
	print_full_vect(v1);
	v1.clear();
	print_full_vect(v3);
	v3.clear();
}

void	test_copy_construct_equal(NAMESPACE::vector<TYPE> &v1, NAMESPACE::vector<TYPE> &v2)
{
	std::cout << "test_copy_construct_equal:" << std::endl;

	NAMESPACE::vector<TYPE> v3(v1);
	print_full_vect(v1);
	print_full_vect(v2);
	print_full_vect(v3);
	v3 = v1;
	print_full_vect(v1);
	print_full_vect(v2);
	print_full_vect(v3);
	v1 = v3;
	v1 = v2;
	v2 = v3;
	v3 = NAMESPACE::vector<TYPE>(v3);
	print_full_vect(v1);
	print_full_vect(v2);
	print_full_vect(v3);
	v3 = NAMESPACE::vector<TYPE>(v1);
}

void	test_construct(NAMESPACE::vector<TYPE> &v1, NAMESPACE::vector<TYPE> &v2)
{
	std::cout << "test_construct:" << std::endl;

	NAMESPACE::vector<TYPE> v4 = NAMESPACE::vector<TYPE>(std::rand() % 20, get_value());
	const std::set<TYPE> s(v1.begin(), v1.end());
	const NAMESPACE::vector<TYPE> v3 = NAMESPACE::vector<TYPE>(s.begin(), s.end());
	v1 = NAMESPACE::vector<TYPE>(v3);
	print_full_vect(v1);
	print_full_vect(v2);
	print_full_vect(v3);
	v1.insert(v1.end(), v4.begin(), v4.end());
}

void 	test_assign(NAMESPACE::vector<TYPE> &v1, NAMESPACE::vector<TYPE> &v2)
{
	std::cout << "test_assign:" << std::endl;

	NAMESPACE::vector<TYPE> v3;

	v3.assign(12, get_value());
	print_full_vect(v1);
	print_full_vect(v2);
	print_full_vect(v3);
	v1.assign(v3.begin(), v3.end() - 1);
	print_full_vect(v1);
	print_full_vect(v2);
	print_full_vect(v3);
}

void	test_at(NAMESPACE::vector<TYPE> &v1, NAMESPACE::vector<TYPE> &v2)
{
	std::cout << "test_at:" << std::endl;

	for (int i = 0; i < 5; i++)
	{
		try {
			if (!v1.empty())
				v1.at((int)std::rand() % v1.size()) = get_value();
			if (!v2.empty())
				std::cout << v2.at((int)std::rand() % v2.size()) << std::endl;
			std::cout << v1.at((unsigned int)std::rand());
			std::cout << v2.at((unsigned int)std::rand());
		}
		catch (std::out_of_range &e)
		{
			std::cout << "out_of_range catched" << std::endl;
		}
	}
	print_full_vect(v1);
	print_full_vect(v2);
}

void	test_operator_index(NAMESPACE::vector<TYPE> &v1, NAMESPACE::vector<TYPE> &v2)
{
	std::cout << "test_operator_index:" << std::endl;

	for (int i = 0; i < 5; i++)
	{
		if (!v1.empty())
			v1.at((int)std::rand() % v1.size()) = get_value();
		if (!v2.empty())
			std::cout << v2.at((int)std::rand() % v2.size()) << std::endl;
	}
	print_full_vect(v1);
	print_full_vect(v2);
}

void	test_insert(NAMESPACE::vector<TYPE> &v1, NAMESPACE::vector<TYPE> &v2)
{
	for (int i = 0; i < 5; i++)
	{
		NAMESPACE::vector<TYPE>::iterator it = v1.begin() + (std::rand() % v1.size());
		switch (std::rand() % 3)
		{
			case (0):
				std::cout << *v1.insert(v1.begin() + (std::rand() % v1.size()), get_value()) << std::endl;
				break;
			case (1):
				v2.insert(v2.begin() + (std::rand() % v2.size()),it, it + (std::rand() % (v1.size() -(it - v1.begin()))));
				break;
			case (2):
				v2.insert(v2.begin() + (std::rand() % v2.size()), std::rand() % 5,get_value());
		}
	}
	print_full_vect(v1);
}

void	test_vector()
{
	void (*array[])(NAMESPACE::vector<TYPE> &, NAMESPACE::vector<TYPE> &) = {
			&test_assign,
			&test_construct,
			&test_copy_construct_equal,
			&test_at,
			&test_operator_index,
			&test_clear,
			&test_insert
	};
	NAMESPACE::vector<TYPE> v1;
	NAMESPACE::vector<TYPE> v2;
	print_full_vect(v1);
	v1.clear();
	print_full_vect(v1);
	for (int i = 0; i < 100; i++)
	{
		int rand = std::rand() % sizeof(array) / sizeof (void (*)(NAMESPACE::vector<TYPE> &, NAMESPACE::vector<TYPE> &));
		if (std::rand() % 2)
			array[rand](v1, v2);
		else
			array[rand](v2, v1);
	}
}