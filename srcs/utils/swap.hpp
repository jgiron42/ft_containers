//
// Created by joachim on 3/24/22.
//

#ifndef FT_CONTAINERS_SWAP_HPP
#define FT_CONTAINERS_SWAP_HPP

namespace ft {
	template<typename T>
	void swap(T &l, T &r) {
		T tmp = l;
		l = r;
		r = tmp;
	}
}

#endif //FT_CONTAINERS_SWAP_HPP
