#include <iostream>
#include "timer.hpp"
#include "is_integral.hpp"
#include "enable_if.hpp"
#include "map.hpp"
#include "vector.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <sys/time.h>
#include <list>
#include "shitty_allocator.hpp"





int main() {

	std::vector<int, shitty_allocator<int> > (12, 14);

	shitty_allocator<int> A;
	shitty_allocator<int> B;
	int * a = A.allocate(1);
	int b = 2;
	A.construct(a, b);
//	delete(a);
	A.destroy(a);
	A.deallocate(a, 1);







/*	std::list<ft::pair<int, int> > l;
	for (int i = 0; i < 10; i++)
		l.push_back(ft::make_pair(i, i));
	ft::map<int, int> m(l.begin(), l.end());
	//ft::map<int, int> m;
	//for (int i = 0; i < 10; i++)
//		m.insert(ft::make_pair(i, i));
	std::cout << &*m.begin() << std::endl;
	std::cout << &*(m.end()) << std::endl;
	ft::map<int, int> m_copy(m);
	std::cout << &*m_copy.begin() << std::endl;
	std::cout << &*(m_copy.end()) << std::endl;
	printSize(m_copy);
	printSize(m);
}
*/
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

//	int *zbeub = new int(12);
//	free(zbeub);
//	ft::map<int, int> m;
//	int tmp;
//	timer<false> t;

//	struct timeval time;
//	gettimeofday(&time, NULL);
//	std::srand(time.tv_usec + (long int)1e6 * time.tv_sec);
//	t.start();
//	for (int i = 0; i < 100; i++)
//	{
//		tmp = std::rand() % 100;
//		m.print();
//		m.insert(ft::make_pair<int, int>(tmp, tmp));
//		m.insert(ft::make_pair<int, int>(i, i));
//		std::cout << "m.insert(ft::make_pair<int, int>(" << tmp << ", " << tmp << ")); " << std::endl;
//	}
//	std::cout << t << std::endl;
//	t.stop();
//	std::cout << t << std::endl;
//	m.print();
//	tmp = std::rand() % 100;
//	std::cout << tmp << std::endl;
//	std::cout << m.lower_bound(tmp)->first << std::endl;
//	std::cout << m.upper_bound(tmp)->first << std::endl;
//
//ft::vector<std::string> a;
//std::vector<int> b;
//
//for (int i = 0; i < 4; i++)
//	b.push_back(i);
//a.insert(a.begin(), 2, "zbeub");
}
