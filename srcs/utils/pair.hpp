//
// Created by Joachim GIRON on 7/18/21.
//

#ifndef FT_CONTAINERS_PAIR_HPP
#define FT_CONTAINERS_PAIR_HPP

#include "type_traits.hpp"

namespace ft {
	template <class T1, class T2> struct pair{
		typedef typename remove_const<T1>::type	first_type;
		typedef typename remove_const<T2>::type	second_type;
		first_type	first;
		second_type	second;
		pair() : first(), second(){};
		pair( const T1& x, const T2& y ) : first(x), second(y) {};
		template< class U1, class U2 >	pair( const pair<U1, U2>& p ) : first(p.first), second(p.second) {};
//		pair( const pair& p ) : first(p.first), second(p.second) {};
		pair& operator=( const pair& other ) {
			this->first = other.first;
			this->second = other.second;
			return (*this);
		};
	};

	template< class T1, class T2 >
	pair<T1,T2> make_pair( T1 t, T2 u ){
		return(pair<T1, T2>(t, u));
	}
	template< class T1, class T2 >
	bool operator==( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs ){
		return (lhs.first == rhs.first && lhs.second == rhs.second);
	}
	template< class T1, class T2 >
	bool operator!=( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs ){
		return (lhs.first != rhs.first || lhs.second != rhs.second);
	};
	template< class T1, class T2 >
	bool operator<( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs ){
		return (lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second));
	};
	template< class T1, class T2 >
	bool operator<=( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs ){
		return (lhs.first < rhs.first || (lhs.first == rhs.first && (lhs.second < rhs.second || lhs.second == rhs.second)));
	};
	template< class T1, class T2 >
	bool operator>( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs ){
		return (rhs.first < lhs.first || (lhs.first == rhs.first && rhs.second < lhs.second));
	};
	template< class T1, class T2 >
	bool operator>=( const ft::pair<T1,T2>& lhs, const ft::pair<T1,T2>& rhs ){
		return (rhs.first < lhs.first || (lhs.first == rhs.first && (rhs.second < lhs.second || lhs.second == rhs.second)));
	};
}



#endif //FT_CONTAINERS_PAIR_HPP
