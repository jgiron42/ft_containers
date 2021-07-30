//
// Created by Joachim GIRON on 7/17/21.
//

#ifndef FT_CONTAINERS_IS_INTEGRAL_HPP
#define FT_CONTAINERS_IS_INTEGRAL_HPP

#include "integral_constant.hpp"

namespace ft {

	typedef integral_constant<bool,true> true_type;
	typedef integral_constant<bool,false> false_type;

	template <typename>
	struct is_integral : public false_type {};
	template <>
	struct is_integral<bool> : public true_type {};
	template <>
	struct is_integral<char> : public true_type {};
	template <>
	struct is_integral<signed char> : public true_type {};
	template <>
	struct is_integral<short> : public true_type {};
	template <>
	struct is_integral<int> : public true_type {};
	template <>
	struct is_integral<long> : public true_type {};
	template <>
	struct is_integral<unsigned char> : public true_type {};
	template <>
	struct is_integral<unsigned short> : public true_type {};
	template <>
	struct is_integral<unsigned int> : public true_type {};
	template <>
	struct is_integral<unsigned long> : public true_type {};

}

#endif //FT_CONTAINERS_IS_INTEGRAL_HPP
