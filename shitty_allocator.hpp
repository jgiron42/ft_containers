//
// Created by jgiron on 11/19/21.
//

#ifndef FT_CONTAINERS_SHITTY_ALLOCATOR_HPP
#define FT_CONTAINERS_SHITTY_ALLOCATOR_HPP

#include <sys/mman.h>
#include <unistd.h>
#include <set>
#include <vector>
#include <memory>
#include <type_traits>

std::allocator<void *> a;
std::vector<std::set<void *, std::less<void *>, std::allocator<void *> >, std::allocator<void *> > shitty_allocator_vector;

template< class T >
class shitty_allocator : private std::allocator<T> {
private:
	std::set<void *> *pool;
public:

	typedef typename std::allocator<T>::value_type				value_type;
	struct propagate_on_container_copy_assignment : public std::true_type  {};


	shitty_allocator() : std::allocator<T>() {
		shitty_allocator_vector.push_back(std::set<void *>());
		this->pool = &shitty_allocator_vector.back();
	}
	template <typename U>
	shitty_allocator(const shitty_allocator<U> &src) : std::allocator<T>(src), pool(src.pool) {}

	shitty_allocator &operator=(shitty_allocator &src)
	{
		this->pool = src.pool;
	}

	typename std::allocator<T>::pointer allocate( typename std::allocator<T>::size_type n, const void * hint = 0 )
	{
		return ((typename std::allocator<T>::pointer)(*pool->insert(mmap((void *)hint, n * sizeof(value_type), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)).first));
	}

	void deallocate( T* p, std::size_t n )
	{
		if (!(pool->count((void *)p)))
		{
			write(2, "shitty_allocator: pointer being freed was not allocated by this allocator\n", 74);
			abort();
		}
		munmap(p, n * sizeof(value_type));
		pool->erase((void*) p);
	}


	template< class T1, class T2 >
	friend bool operator==( const shitty_allocator<T1>& lhs, const shitty_allocator<T2>& rhs ) throw() {return (lhs.pool == rhs.pool);};
	template< class T1, class T2 >
	friend bool operator!=( const shitty_allocator<T1>& lhs, const shitty_allocator<T2>& rhs ) throw() {return (lhs.pool != rhs.pool);};
};

#endif //FT_CONTAINERS_SHITTY_ALLOCATOR_HPP
