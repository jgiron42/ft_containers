//
// Created by jgiron on 7/17/21.
//

#ifndef FT_CONTAINERS_ITERATORS_TRAITS_HPP
#define FT_CONTAINERS_ITERATORS_TRAITS_HPP
#include <iterator>
namespace ft {
	template< class Iter >
	struct iterator_traits{
		typedef typename Iter::difference_type difference_type;
		typedef typename Iter::value_type value_type;
		typedef	typename Iter::pointer pointer;
		typedef	typename Iter::reference reference;
		typedef typename Iter::iterator_category iterator_category;
	};
	template< class T >
	struct iterator_traits<T*>{
		typedef 	std::ptrdiff_t difference_type;
		typedef 			T* pointer;
		typedef 			T& reference;
		typedef 	std::random_access_iterator_tag iterator_category;
	};
}

#endif //FT_CONTAINERS_ITERATORS_TRAITS_HPP
