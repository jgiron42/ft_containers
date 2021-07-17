//
// Created by jgiron on 7/17/21.
//

#ifndef FT_CONTAINERS_REVERSE_ITERATOR_HPP
#define FT_CONTAINERS_REVERSE_ITERATOR_HPP

#include "iterators_traits.hpp"

namespace ft {
	template< class Iter >
	class reverse_iterator{
		typedef Iter											iterator_type;
		typedef typename ft::iterator_traits<Iter>::iterator_category	iterator_category;
		typedef typename ft::iterator_traits<Iter>::value_type			value_type;
		typedef typename ft::iterator_traits<Iter>::difference_type		difference_type;
		typedef typename ft::iterator_traits<Iter>::pointer				pointer;
		typedef typename ft::iterator_traits<Iter>::reference			reference;
	public:
		reverse_iterator() : current() {};
		explicit reverse_iterator(iterator_type x ) : current(x) {};
		template< class U >
		reverse_iterator( const reverse_iterator<U>& other ) : current(other.base()) {};
		template< class U >
		reverse_iterator& operator=( const reverse_iterator<U>& other ) {return(this->current = other.base());};
		iterator_type base() const{	return (this->current);	};
		reference operator*() const {
			Iter tmp(this->current);
			return (*--tmp);
		};
		pointer operator->() const {
			Iter tmp(this->current);
			return (*--tmp);
		};
		reference operator[]( difference_type n ) const {
			return (base()[-n-1]);
		};
		reverse_iterator& operator++() {
			return (--current);
		};
		reverse_iterator& operator--() {
			return (++current);
		};;
		reverse_iterator operator++( int ){
			return (current--);
		};;
		reverse_iterator operator--( int ){
			return (current++);
		};;
		reverse_iterator operator+( difference_type n ) const{
			return (current - n);
		};;
		reverse_iterator operator-( difference_type n ) const {
			return (current + n);
		};
		reverse_iterator& operator+=( difference_type n ) {
			return (current -= n);
		};
		reverse_iterator& operator-=( difference_type n ) {
			return (current += n);
		};
	protected:
		iterator_type current;
	};
	template< class Iterator1, class Iterator2 >
	bool operator==( const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs ) {
		return (lhs.base() == rhs.base());
	};

	template< class Iterator1, class Iterator2 >
	bool operator!=( const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs ) {
		return (lhs.base() != rhs.base());
	};

	template< class Iterator1, class Iterator2 >
	bool operator<( const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs ) {
		return (lhs.base() > rhs.base());
	};

	template< class Iterator1, class Iterator2 >
	bool operator<=( const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs ) {
		return (lhs.base() >= rhs.base());
	};

	template< class Iterator1, class Iterator2 >
	bool operator>( const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs ) {
		return (lhs.base() < rhs.base());
	};

	template< class Iterator1, class Iterator2 >
	bool operator>=( const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs ) {
		return (lhs.base() <= rhs.base());
	};

	template< class Iter >
	reverse_iterator<Iter> operator+( typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& it ) {
		return (reverse_iterator<Iter>(it.base() - n));
	};

	template< class Iterator >
	typename reverse_iterator<Iterator>::difference_type operator-( const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs ) {
		return (rhs.base() - lhs.base());
	};

}

#endif //FT_CONTAINERS_REVERSE_ITERATOR_HPP
