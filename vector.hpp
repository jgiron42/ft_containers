//
// Created by Joachim GIRON on 7/18/21.
//

#ifndef FT_CONTAINERS_VECTOR_HPP
#define FT_CONTAINERS_VECTOR_HPP

#include "reverse_iterator.hpp"
#include "lexicographical_compare.hpp"
#include "equal.hpp"
#include "iterator.hpp"
#include "enable_if.hpp"
#include "is_integral.hpp"
#include <iostream>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#define SSTR( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

namespace ft {
	typedef int ptrdiff_t;
	typedef unsigned int size_t;

	template<
			class T,
			class Allocator = std::allocator <T>
	>
	class vector {
	public:
		typedef T value_type;
		typedef Allocator allocator_type;
		typedef size_t size_type;
		typedef iterator_traits<iterator>::difference_type difference_type;
		typedef allocator_type::reference &reference;
		typedef allocator_type::const_reference const_reference;
		typedef allocator_type::pointer pointer;
		typedef allocator_type::const_pointer const_pointer;
	private:
		template <typename>
		struct get_pointer {};
		template <>
		struct get_pointer<value_type> {typedef pointer type;};
		template <>
		struct get_pointer<const value_type> {typedef const_pointer type;};
		template<typename U>
		class t_iterator {
		public:
			typedef ft::ptrdiff_t					difference_type;
			typedef U								value_type;
			typedef	get_pointer<U>					pointer;
			typedef	U &							reference;
			typedef ft::random_access_iterator_tag iterator_category;
		protected:
			U *p;
		public:
			template <typename V>
			t_iterator(t_iterator<V> const &src) : p((value_type *)src.operator->()) {}
			t_iterator(U *src) : p(src) {}
			~t_iterator() {}
			template <typename V>
			t_iterator &operator=(t_iterator<V> &src) { this->p = src.operator->(); }
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
			friend t_iterator operator+(t_iterator lhs, int rhs) {return (t_iterator(lhs.p + rhs));}
			friend t_iterator operator+(int lhs, t_iterator rhs) {return (t_iterator(rhs.p + lhs));}
			friend t_iterator operator-(t_iterator lhs, int rhs) {return (t_iterator(lhs.p - rhs));}
			friend size_type operator-(t_iterator lhs, t_iterator rhs) {return (lhs.p - rhs.p);}
			U &operator[](int n) {return (p[n]);}
			t_iterator &operator+=(int n) {
				this->p += n;
				return (*this);
			}
			t_iterator &operator-=(int n) {
				this->p -= n;
				return (*this);
			}
			template <typename V>
			bool operator==(const t_iterator<V> &rhs) {
				return (this->p == rhs.operator->());
			}
			template <typename V>
			bool operator!=(const t_iterator<V> &rhs) {
				return (this->p != rhs.operator->());
			}
			template <typename V>
			bool operator<(const t_iterator<V> &rhs) {
				return (this->p < rhs.operator->());
			}
			template <typename V>
			bool operator>(const t_iterator<V> &rhs) {
				return (this->p > rhs.operator->());
			}
			template <typename V>
			bool operator<=(const t_iterator<V> &rhs) {
				return (this->p <= rhs.operator->());
			}
			template <typename V>
			bool operator>=(const t_iterator<V> &rhs) {
				return (this->p >= rhs.operator->());
			}
		};
	public:
		typedef t_iterator<T> iterator;
		typedef t_iterator<const T> const_iterator;
		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
	private:
		allocator_type A;
		pointer				data;
		size_type _capacity;
		size_type _size;
	public:
		vector() : A(allocator_type()), data(A.allocate(0)), _capacity(0), _size(0) {}

		explicit vector(const Allocator &alloc) : A(alloc), data(A.allocate(1)), _capacity(1), _size(0) {}

		explicit vector(size_type count, const T &value = T(), const Allocator &alloc = Allocator())
		: A(alloc), data(A.allocate(count)), _capacity(count), _size(count) {
			for (size_type i = 0; i < count; i++)
				this->data[i] = value;
		}

		template< class InputIt >
		vector( InputIt first, typename ft::enable_if<!is_integral<InputIt>::value, InputIt>::type last, const Allocator& alloc = Allocator()) : A(alloc), data(A.allocate(last - first)), _capacity(last - first), _size(0)  {
			size_type count = last - first;
			this->_size = count;
			int j = 0;
			for (InputIt i(first); i != last; i++)
				this->data[j++] = *i;
		}

		template< class InputIt >
		vector( InputIt first, typename ft::enable_if<is_integral<InputIt>::value, InputIt>::type last, const Allocator& alloc = Allocator()) : A(alloc), data(A.allocate(static_cast<size_type>(first))), _capacity(static_cast<size_type>(first)), _size(static_cast<size_type>(first)) {
			for (size_type i = 0; i < static_cast<size_type>(first); i++)
				this->data[i] = static_cast<value_type>(last);
		}

		vector(const vector &other) : A(allocator_type()), data(A.allocate(other._capacity)),
									  _capacity(other._capacity), _size(other._size) {
			for (size_type i = 0; i < other._size; i++)
				(this->data)[i] = T(other.data[i]);
		}
		~vector() {
			this->A.deallocate(this->data, this->_capacity);
		}
		vector &operator=(const vector &other) {
			this->A.deallocate(this->data, this->_capacity);
			this->data = this->A.allocate(other._capacity);
			this->_capacity = other._capacity;
			this->_size = other._size;
			for (size_type i = 0; i < other._size; i++)
				this->data[i] = T(other.data[i]);
			return (*this);
		}

		void assign(size_type count, const T &value) {
			if (count > _capacity) {
				this->A.deallocate(this->data, this->_capacity);
				this->data = this->A.allocate(count);
				this->_capacity = count;
			}
			this->_size = count;
			for (size_type i = 0; i < count; i++)
				this->data[i] = T(value);
		}

		template< class InputIt >
		void assign( InputIt first, InputIt last ) {
			size_type count = last - first;
			if (count > _capacity) {
				this->A.deallocate(this->data, this->_capacity);
				this->data = this->A.allocate(count);
				this->_capacity = count;
			}
			this->_size = count;
			int j = 0;
			for (InputIt i(first); i != last; i++)
				this->data[j++] = *i;
		}

		allocator_type get_allocator() const { return this->A; }

		reference at(size_type pos) {
			range_check(pos);
			return (this->data[pos]);
		}

		const_reference at(size_type pos) const {
			range_check(pos);
			return (this->data[pos]);
		}

		reference operator[](size_type pos) {
			return (this->data[pos]);
		}

		const_reference operator[](size_type pos) const {
			return (this->data[pos]);
		}

		reference front() {
			return (this->data[0]);
		}

		const_reference front() const {
			return (this->data[0]);
		}

		reference back() {
			return (this->data[this->_size - 1]);
		}

		const_reference back() const {
			return (this->data[this->_size - 1]);
		}

		iterator begin() {
			return (iterator(this->data));
		}

		const_iterator begin() const {
			return (iterator(this->data));
		}

		iterator end() {
			return (iterator(this->data + _size));
		}

		const_iterator end() const {
			return (iterator(this->data + _size));
		}

		reverse_iterator rbegin() {
			return (reverse_iterator(this->data + _size));
		}

		const_reverse_iterator rbegin() const {
			return (const_reverse_iterator(this->data + _size));
		}

		reverse_iterator rend() {
			return (reverse_iterator(this->data));
		}

		const_reverse_iterator rend() const {
			return (const_reverse_iterator(this->data));
		}

		bool empty() const {
			return (!!this->_size);
		}

		size_type size() const {
			return (this->_size);
		}

		size_type max_size() const {
			return (std::numeric_limits<difference_type>::max());
		}

		void reserve(size_type new_cap) {
			if (new_cap <= this->_size)
				return;
			pointer tmp = A.allocate(new_cap);
//			std::cerr << "pointer data: " << data << "\npointer tmp: " << tmp << "\nnew_cap: " << new_cap << "\nsize: " << _size << "\ncapacity: " << _capacity << std::endl;
			for (size_type i = 0; i < this->_size; i++) {
//				std::cerr << "-> " << i << std::endl;
//				std::cerr << "tmp" << std::endl;
//				(void)tmp[i];
//				std::cerr << "data" << std::endl;
//				(void)data[i];
				tmp[i] = this->data[i];
			}
			A.deallocate(this->data, this->_size);
			this->_capacity = new_cap;
			this->data = tmp;
		}

		size_type capacity() const {
			return (this->_capacity);
		}

		iterator insert( iterator pos, const T& value ){
			size_type tmp = pos - this->begin();
			if (this->_size + 1 > this->_capacity)
				this->reserve(this->_size + 1);
			pos = this->begin() + tmp;
			this->_size++;
			for (iterator i = this->end(); i != pos + 1; i--)
				*(i - 1) = *(i - 2);
			*pos = value;
			return (pos);
		}

		void insert( iterator pos, size_type count, const T& value ) {
			if (!count || pos > this->end() || pos < this->begin())
				return;
			size_type tmp = pos - this->begin();
			if (this->_size + count > this->_capacity)
				this->reserve(this->_size + count);
			pos = this->begin() + tmp;
			for (int i = this->end() - pos - 1; i > 0; i--)
				pos[i + count] = pos[i];
			this->_size += count;
			while (--count + 1 > 0)
				pos[count] = T(value);
		}


		template<class InputIt>
		void insert( iterator pos, typename ft::enable_if<!is_integral<InputIt>::value, InputIt>::type first, typename ft::enable_if<!is_integral<InputIt>::value, InputIt>::type last) {
			int count = last - first;
			size_type tmp = pos - this->begin();
			if (this->_size + count > this->_capacity)
				this->reserve(this->_size + count);
			pos = this->begin() + tmp;
			for (int i = this->end() - pos - 1; i > 0; i--)
				pos[i + count] = pos[i];
			this->_size += count;
			while (--count + 1 > 0)
				pos[count] = *(--last);
		}

		template<class InputIt>
		void insert( iterator pos, typename ft::enable_if<is_integral<InputIt>::value, InputIt>::type first, typename ft::enable_if<is_integral<InputIt>::value, InputIt>::type last) {
				this->insert(pos, static_cast<size_type>(first), static_cast<value_type>(last));
		}

		void resize( size_type count, T value = T() ) {
			if (count < this->_size)
				this->_size = count;
			else if (count > this->_size)
			{
				if (count > _capacity)
					this->reserve(count);
				for (long int i = (long int)_size; i < (long int)count; i++)
					this->data[i] = T(value);
			}
		}

		void clear() {
			this->_size = 0;
		}

		iterator erase(iterator pos) {
			iterator ret = pos + 1;
			while (pos + 1 != this->end())
			{
				pos[0] = pos[1];
				pos++;
			}
			this->_size--;
			return (ret);
		}

		iterator erase( iterator first, iterator last ) {
			iterator ret = first;
			while (last != this->end())
			{
				*first = *last;
				last++;
				first++;
			}
			this->_size = last - first;
			return (ret);
		}
		void push_back(const T &value) {
			if (this->_size == this->_capacity)
				this->reserve(this->_capacity == 0 ? 1 : this->_capacity * 2);
			this->data[this->_size] = value;
			this->_size++;
		}

		void pop_back() {
			this->_size--;
		}

		void swap(vector &other) {
			allocator_type tmpalloc = this->A;
			this->A = other.A;
			other.A = tmpalloc;
			pointer tmpdata = this->data;
			this->data = other.data;
			other.data = tmpdata;
			int tmpsize = this->_size;
			this->_size = other._size;
			other._size = tmpsize;
			tmpsize = this->_capacity;
			this->_capacity = other._capacity;
			other._capacity = tmpsize;
		}

		friend bool operator==(const vector &lhs, const vector &rhs) {
			return (ft::equal<iterator , iterator>(lhs.begin(), lhs.end(), rhs.begin(),	rhs.end()));
		}
		friend bool operator!=(const vector &lhs, const vector &rhs) {
			return (!ft::equal<iterator, iterator>(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
		}
		friend bool operator<(const vector &lhs, const vector &rhs) {
			return (ft::lexicographical_compare<iterator, iterator>(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
		}
		friend bool operator<=(const vector &lhs, const vector &rhs) {
			return (!ft::lexicographical_compare<iterator, iterator>(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));
		}
		friend bool operator>(const vector &lhs, const vector &rhs) {
			return (ft::lexicographical_compare<iterator, iterator>(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));
		}
		friend bool operator>=(const vector &lhs, const vector &rhs) {
			return (!(ft::lexicographical_compare<iterator, iterator>(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())));
		}

	private:
		void range_check(size_type n) const {
			if (n < 0)
				throw std::out_of_range(SSTR("vector::range_check n (which is " << n << ") < 0"));
			if (n >= this->_size)
				throw std::out_of_range(SSTR("vector::range_check n (which is " << n << ") >= this.size() (which_is " << this->_size	<< ")"));
		}
	};
}

#endif //FT_CONTAINERS_VECTOR_HPP
