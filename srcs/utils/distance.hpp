//
// Created by Joachim GIRON on 10/25/21.
//

#ifndef FT_CONTAINERS_DISTANCE_HPP
#define FT_CONTAINERS_DISTANCE_HPP
#include "iterators_traits.hpp"
#include <iterator>
#include "iterator.hpp"

namespace ft {


	template <typename InputIt>
	typename ft::iterator_traits<InputIt>::difference_type do_distance(InputIt first, InputIt last, std::input_iterator_tag)
	{
		typename ft::iterator_traits<InputIt>::difference_type ret = 0;

		for (;first != last; ++first)
			++ret;
		return(ret);
	}

	template <typename InputIt>
	typename ft::iterator_traits<InputIt>::difference_type do_distance(InputIt first, InputIt last, std::random_access_iterator_tag)
	{
		return(last - first);
	}

	template <typename InputIt>
	typename ft::iterator_traits<InputIt>::difference_type distance (InputIt first, InputIt last)
	{
		return (do_distance(first, last, typename ft::iterator_traits<InputIt>::iterator_category()));
	}
}

#endif //FT_CONTAINERS_DISTANCE_HPP
