//
// Created by jgiron on 11/21/21.
//

#ifndef FT_CONTAINERS_BASIC_ALLOCATOR_HPP
#define FT_CONTAINERS_BASIC_ALLOCATOR_HPP

#include <memory>

template <typename T>
class basic_allocator : private std::allocator<T> {
private:
	typedef std::allocator<T> base;
public:
	typedef typename base::value_type value_type;
	basic_allocator() throw() : base() {};
	basic_allocator( const basic_allocator& other ) throw() : base(other) {};
	template< class U >
	basic_allocator( const basic_allocator<U>& other ) throw() : base(other) {};

	using base::allocate;
	using base::deallocate;

	template< class T1, class T2 >
	friend bool operator==( const basic_allocator<T1>& lhs, const basic_allocator<T2>& rhs ) throw() {return (lhs.base == rhs.base);};
	template< class T1, class T2 >
	friend bool operator!=( const basic_allocator<T1>& lhs, const basic_allocator<T2>& rhs ) throw() {return (lhs.base != rhs.base);};
};

#endif //FT_CONTAINERS_BASIC_ALLOCATOR_HPP
