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

std::vector<std::set<void *> > shitty_allocator_vector;

template< class T >
class shitty_allocator : private std::allocator<T> {
private:
	std::set<void *> *pool;
public:

	typedef typename std::allocator<T>::pointer				pointer;
	typedef typename std::allocator<T>::const_pointer		const_pointer;
	typedef typename std::allocator<T>::value_type				value_type;
	typedef typename std::allocator<T>::size_type			size_type;
	typedef typename std::allocator<T>::difference_type			difference_type;
	typedef typename std::allocator<T>::reference			reference;
	typedef typename std::allocator<T>::const_reference			const_reference;
	typedef typename std::allocator<T>::propagate_on_container_move_assignment			propagate_on_container_move_assignment;
	typedef std::false_type							is_always_equal;
	template<typename U>
	struct rebind {
		typedef shitty_allocator<U> other;
	};


	shitty_allocator() : std::allocator<T>() {
		shitty_allocator_vector.push_back(std::set<void *>());
		this->pool = &shitty_allocator_vector.back();
	}
	template <typename U>
	shitty_allocator(const shitty_allocator<U> &src) : std::allocator<T>(src), pool(src.pool) {}

	typename std::allocator<T>::pointer allocate( typename std::allocator<T>::size_type n, const void * hint = 0 )
	{
		return ((typename std::allocator<T>::pointer)(*pool->insert((void *)std::allocator<T>::allocate(n, hint)).first));
	}

	void deallocate( T* p, std::size_t n )
	{
		if (!pool->count((void *)p))
		{
			write(2, "shitty_allocator: pointer being freed was not allocated by this allocator\n", 74);
			abort();
		}
		std::allocator<T>::deallocate(p, n);
		pool->erase((void*) p);
	}

	using std::allocator<T>::construct;
	using std::allocator<T>::destroy;
	using std::allocator<T>::max_size;

	template< class T1, class T2 >
	friend bool operator==( const shitty_allocator<T1>& lhs, const shitty_allocator<T2>& rhs ) throw() {return (lhs.pool == rhs.pool);};
	template< class T1, class T2 >
	friend bool operator!=( const shitty_allocator<T1>& lhs, const shitty_allocator<T2>& rhs ) throw() {return (lhs.pool != rhs.pool);};
};

#endif //FT_CONTAINERS_SHITTY_ALLOCATOR_HPP
