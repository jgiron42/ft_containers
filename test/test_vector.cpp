
#ifndef NAMESPACE
#define NAMESPACE ft
#endif
#ifndef TYPE
#define TYPE std::string
#endif

void	print_full_vect(NAMESPACE::vector<TYPE> &v)
{
	std::cout << "empty: " << v.empty() << std::endl;
	std::cout << "size: " << v.size() << std::endl;
	std::cout << "capacity: " << v.capacity() << std::endl;
	std::cout << "front: " << v.front() << std::endl;
	std::cout << "back: " << v.back() << std::endl;
	std::cout << "content:" << std::endl;
	for (NAMESPACE::vector<TYPE>::iterator i = v.begin(); i != v.end(); i++)
		std::cout << " " << *i << std::endl;
	std::cout << "reverse content:" << std::endl;
	for (NAMESPACE::vector<TYPE>::reverse_iterator i = v.rbegin(); i != v.rend(); i++)
		std::cout << " " << *i << std::endl;
}

void	test_copy_construct_equal(NAMESPACE::vector<TYPE> &v1, NAMESPACE::vector<TYPE> &v2)
{
	NAMESPACE::vector<TYPE> v3(v1);
	print_full_vect(v1);
	print_full_vect(v2);
	print_full_vect(v3);
	v3 = v1;
	print_full_vect(v1);
	print_full_vect(v2);
	print_full_vect(v3);
	v1 = v3;
	v1 = v2
	v2 = v3;
	v3 = v2(v3);
	print_full_vect(v1);
	print_full_vect(v2);
	print_full_vect(v3);
	v3 = NAMESPACE::vector<TYPE>(v1);
}

void	test_construct(NAMESPACE::vector<TYPE> &v1, NAMESPACE::vector<TYPE> &v2)
{
	NAMESPACE::vector<TYPE> v4 = NAMESPACE::vector<TYPE>(std::rand() % 100, TYPE());
	const std::set<TYPE> s(v1.begin(), v1.end());
	const NAMESPACE::vector<TYPE> v3 = NAMESPACE::vector<TYPE>(s.begin(), s.end());
	v1 = NAMESPACE::vector<TYPE>(v3);
	v1.insert(v1.end(), v4.begin(), v4.end());
}

void 	test_assign(NAMESPACE::vector<TYPE> &v1, NAMESPACE::vector<TYPE> &v2)
{
	NAMESPACE::vector<TYPE> v3;

	v3.assign(12, TYPE());
}

void	test_vector()
{
	NAMESPACE::vector<TYPE> v1;
	NAMESPACE::vector<TYPE> v2;
	print_full_vect(v1);
	v.clear()
	print_full_vect(v1);
	for (int i = 0; i < 100; i++)
	{
		if (std::rand() % 2)
			array[std::rand() % sizeof(array)](v1, v2);
		else
			array[std::rand() % sizeof(array)](v2, v1);
	}
}