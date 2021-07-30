//
// Created by Joachim GIRON on 7/17/21.
//

#ifndef FT_CONTAINERS_INTEGRAL_CONSTANT_HPP
#define FT_CONTAINERS_INTEGRAL_CONSTANT_HPP

namespace ft {
	template <class T, T v>
	struct integral_constant {
		const static T value = v;
		typedef T value_type;
		typedef integral_constant<T, v>	type;
		operator T() {return v;}
	};
}

#endif //FT_CONTAINERS_INTEGRAL_CONSTANT_HPP
