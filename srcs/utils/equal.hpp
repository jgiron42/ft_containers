//
// Created by Joachim GIRON on 7/19/21.
//

#ifndef FT_CONTAINERS_EQUAL_HPP
#define FT_CONTAINERS_EQUAL_HPP

namespace ft {
	template<class InputIt1, class InputIt2>
	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			if (!(*first1 == *first2)) {
				return false;
			}
		}
		return (first1 == last1 && first2 == last2);
	};

//	template<class InputIt1, class InputIt2, class BinaryPredicate>
//	bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p)
//	{
//		for (; first1 != last1; ++first1, ++first2) {
//			if (!p(*first1, *first2)) {
//				return false;
//			}
//		}
//		return true;
//	}
}

#endif //FT_CONTAINERS_EQUAL_HPP
