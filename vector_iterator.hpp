//
// Created by Joachim GIRON on 10/25/21.
//

#ifndef FT_CONTAINERS_VECTOR_ITERATOR_HPP
#define FT_CONTAINERS_VECTOR_ITERATOR_HPP

#include <cstddef>

namespace ft
{
	template<typename U>
	class t_iterator {
	public:
		typedef std::ptrdiff_t					difference_type;
		typedef U								value_type;
		typedef	U*							pointer;
		typedef	U &							reference;
		typedef std::random_access_iterator_tag iterator_category;
	protected:
		U *p;
	public:
		template <typename V>

		t_iterator(t_iterator<V> const &src) : p((value_type *)src.operator->()) {}
		t_iterator(U *src) : p(src) {}
		t_iterator() : p(0) {}
		~t_iterator() {}
		template <typename V>
		t_iterator &operator=(t_iterator<V> &src) {
			this->p = src.operator->();
			return (*this);
		}
		U &operator*() { return (*p); }
		U *operator->() const { return (p); }
		t_iterator &operator++() {
			++p;
			return (*this);
		}
		t_iterator operator++(int) {
			t_iterator tmp = *this;
			p++;
			return (tmp);
		}
		t_iterator &operator--() {
			--p;
			return (*this);
		}
		t_iterator operator--(int) {
			t_iterator tmp = *this;
			p--;
			return (tmp);
		}
		operator t_iterator<const value_type>() const {return (t_iterator<const value_type>(this->p));}
		U &operator[](int n) {return (p[n]);}
		t_iterator &operator+=(int n) {
			this->p += n;
			return (*this);
		}
		t_iterator &operator-=(int n) {
			this->p -= n;
			return (*this);
		}
	};
	template <typename L, typename R>
	bool operator==(const t_iterator<L> &lhs, const t_iterator<R> &rhs) {
		return (lhs.operator->() == rhs.operator->());
	}
	template <typename L, typename R>
	bool operator!=(const t_iterator<L> &lhs, const t_iterator<R> &rhs) {
		return (lhs.operator->() != rhs.operator->());
	}
	template <typename L, typename R>
	bool operator<(const t_iterator<L> &lhs, const t_iterator<R> &rhs) {
		return (lhs.operator->() < rhs.operator->());
	}
	template <typename L, typename R>
	bool operator>(const t_iterator<L> lhs, const t_iterator<R> &rhs) {
		return (lhs.operator->() > rhs.operator->());
	}
	template <typename L, typename R>
	bool operator<=(const t_iterator<L> &lhs, const t_iterator<R> &rhs) {
		return (lhs.operator->() <= rhs.operator->());
	}
	template <typename L, typename R>
	bool operator>=(const t_iterator<L> &lhs, const t_iterator<R> &rhs) {
		return (lhs.operator->() >= rhs.operator->());
	}
	template <typename V>
	t_iterator<V> operator+(t_iterator<V> lhs, typename t_iterator<V>::difference_type rhs) {return (t_iterator<V>(lhs.operator->() + rhs));}
	template <typename V>
	t_iterator<V> operator+(typename t_iterator<V>::difference_type lhs, t_iterator<V> &rhs) {return (t_iterator<V>(rhs.operator->() + lhs));}
	template <typename V>
	t_iterator<V> operator-(t_iterator<V> lhs, typename t_iterator<V>::difference_type rhs) {return (t_iterator<V>(lhs.operator->() - rhs));}
	template <typename L, typename R>
	typename t_iterator<L>::difference_type operator-(t_iterator<L> lhs, t_iterator<R> rhs) {return (lhs.operator->() - rhs.operator->());}
}
#endif //FT_CONTAINERS_VECTOR_ITERATOR_HPP
