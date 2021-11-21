//
// Created by Joachim GIRON on 7/18/21.
//

#ifndef FT_CONTAINERS_VECTOR_HPP
#define FT_CONTAINERS_VECTOR_HPP

#include "reverse_iterator.hpp"
#include "lexicographical_compare.hpp"
#include "equal.hpp"
#include "iterator.hpp"
#include "distance.hpp"
#include "enable_if.hpp"
#include "is_integral.hpp"
#include "vector_iterator.hpp"
#include <iterator>
#include <iostream>
#include <memory>
#include <ext/alloc_traits.h>
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
		typedef value_type &reference;
		typedef const value_type &const_reference;
//		typedef typename ft::allocator_traits<allocator_type>::pointer pointer;
//		typedef typename ft::allocator_traits<allocator_type>::const_pointer const_pointer;
	private:

	public:
		typedef t_iterator<T> iterator;
		typedef t_iterator<const T> const_iterator;
		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef typename iterator_traits<iterator>::difference_type difference_type;
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
				this->A.construct(this->data + i, value);
//				ft::allocator_traits<allocator_type>::construct(this->A, this->data + i, value);
		}

		template< class InputIt >
		vector( InputIt first, typename ft::enable_if<!is_integral<InputIt>::value, InputIt>::type last, const Allocator& alloc = Allocator()) : A(alloc) {
			this->_size = ft::distance(first, last);
			this->data = A.allocate(this->_size);
			this->_capacity = this->_size;
			int j = 0;
			for (InputIt i(first); i != last; i++)
				this->A.construct(this->data + j++, *i);
//				ft::allocator_traits<allocator_type>::construct(this->A, this->data + j++, *i);
		}

		template< class InputIt >
		vector( InputIt first, typename ft::enable_if<is_integral<InputIt>::value, InputIt>::type last, const Allocator& alloc = Allocator()) :
		A(alloc), data(A.allocate(static_cast<size_type>(first))), _capacity(static_cast<size_type>(first)), _size(static_cast<size_type>(first)) {
			for (size_type i = 0; i < static_cast<size_type>(first); i++)
				this->A.construct(this->data + i, static_cast<value_type>(last));
//				ft::allocator_traits<allocator_type>::construct(this->A, this->data + i, static_cast<value_type>(last));
		}

//		vector(const vector &other) : A(ft::allocator_traits<allocator_type>::select_on_container_copy_construction(other.A)), data(A.allocate(other._capacity)),
		vector(const vector &other) : A(other.A), data(A.allocate(other._capacity)),
									  _capacity(other._capacity), _size(other._size) {
			for (size_type i = 0; i < other._size; i++)
				this->A.construct(this->data + i, T(other.data[i]));
//				ft::allocator_traits<allocator_type>::construct(this->A, this->data + i, T(other.data[i]));
		}
		~vector() {
			this->clear();
			this->A.deallocate(this->data, this->_capacity);
		}
		vector &operator=(const vector &other) {
			this->A.deallocate(this->data, this->_capacity);
			this->data = this->A.allocate(other._capacity);
//			if (ft::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value)
				this->A = other.A;
			this->_capacity = other._capacity;
			this->_size = other._size;
			for (size_type i = 0; i < other._size; i++)
				this->A.construct(this->data + i, T(other.data[i]));
//				ft::allocator_traits<allocator_type>::construct(this->A, this->data + i, T(other.data[i]));
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
				this->A.construct(this->data + i, T(value));
//				ft::allocator_traits<allocator_type>::construct(this->A, this->data + i, T(value));
		}

		template< class InputIt >
		void assign( InputIt first, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type last ) {
			size_type count = ft::distance(first, last);

			if (count > _capacity) {
				this->A.deallocate(this->data, this->_capacity);
				this->data = this->A.allocate(count);
				this->_capacity = count;
			}
			this->_size = count;
			int j = 0;
			for (InputIt i(first); i != last; i++)
				this->A.construct(this->data + j++, *i);
//				ft::allocator_traits<allocator_type>::construct(this->A, this->data + j++, *i);
		}

		allocator_type get_allocator() const { return allocator_type(this->A); }

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
			return (const_iterator(this->data));
		}

		iterator end() {
			return (iterator(this->data + _size ));
		}

		const_iterator end() const {
			return (const_iterator(this->data + _size));
		}

		reverse_iterator rbegin() {
			return (reverse_iterator(this->end()));
		}

		const_reverse_iterator rbegin() const {
			return (const_reverse_iterator(this->end()));
		}

		reverse_iterator rend() {
			return (reverse_iterator(this->begin()));
		}

		const_reverse_iterator rend() const {
			return (const_reverse_iterator(this->begin()));
		}

		bool empty() const {
			return (!this->_size);
		}

		size_type size() const {
			return (this->_size);
		}

		size_type max_size() const {
//			return (ft::allocator_traits<allocator_type>::max_size(this->A));
			return (this->A.max_size());
		}

		void reserve(size_type new_cap) {
			if (new_cap <= this->_size)
				return;
			if (new_cap > this->max_size())
				throw std::length_error("vector::reserve");
			pointer tmp = A.allocate(new_cap);
			for (size_type i = 0; i < this->_size; i++)
				this->A.construct(tmp + i, this->data[i]);
//				ft::allocator_traits<allocator_type>::construct(this->A, tmp + i, this->data[i]);
			A.deallocate(this->data, this->_capacity);
			this->_capacity = new_cap;
			this->data = tmp;
		}

		size_type capacity() const {
			return (this->_capacity);
		}

		iterator insert( iterator pos, const T& value ) {
			size_type tmp = pos - this->begin();
			if (this->_size + 1 > this->_capacity)
				this->reserve(this->_size + 1);
			pos = this->data + tmp;
			for (iterator i = this->end(); i > pos; i--)
				if (i < this->end())
					i[0] = i[-1];
				else
					this->A.construct(i.operator->(), T(i[-1]));
//					ft::allocator_traits<allocator_type>::construct(this->A, i.operator->(), T(i[-1]));
			if (pos < this->end())
				*(pos) = T(value);
			else
				this->A.construct(pos.operator->(), T(value));
//				ft::allocator_traits<allocator_type>::construct(this->A, pos.operator->(), T(value));
			this->_size++;
			return (pos);
		}

		void insert( iterator pos, size_type count, const T& value ) {
			if (!count)
				return;
			size_type tmp = pos - this->begin();
			if (this->_size + count > this->_capacity)
				this->reserve(this->_size + count);
			pos = this->data + tmp;
			for (iterator i = this->end() - 1; i >= pos; i--)
				if (i + count < this->end())
					i[count] = *i;
				else
					this->A.construct(i.operator->() + count, T(*i));
//					ft::allocator_traits<allocator_type>::construct(this->A, i.operator->() + count, T(*i));
			for (size_type i = count; i > 0; i--)
				if (pos.operator->() + i - 1 < this->end().operator->())
					*(pos.operator->() + i - 1 ) = T(value);
				else
					this->A.construct(pos.operator->() + i - 1, T(value));
//					ft::allocator_traits<allocator_type>::construct(this->A, pos.operator->() + i - 1, T(value));
			this->_size += count;

		}


		template<class InputIt>
		void insert( iterator pos, typename ft::enable_if<!is_integral<InputIt>::value, InputIt>::type first, InputIt last) {
			int count = ft::distance(first, last);
			if (!count)
				return;
			size_type tmp = pos - this->begin();
			if (this->_size + count > this->_capacity)
				this->reserve(this->_size + count);
			pos = this->data + tmp;
			for (iterator i = this->end() - 1; i >= pos; i--)
				if (i + count < this->end())
					i[count] = *i;
				else
					this->A.construct(i.operator->() + count, T(*i));
//					ft::allocator_traits<allocator_type>::construct(this->A, i.operator->() + count, T(*i));
			for (size_type i = count; i > 0; i--)
				if (pos.operator->() + i - 1 < this->end().operator->())
					*(pos.operator->() + i - 1 ) = *(--last);
				else
					this->A.construct(pos.operator->() + i - 1, *(--last));
//					ft::allocator_traits<allocator_type>::construct(this->A, pos.operator->() + i - 1, *(--last));
			this->_size += count;
		}

		template<class InputIt>
		void insert( iterator pos, typename ft::enable_if<is_integral<InputIt>::value, InputIt>::type first, InputIt last) {
				this->insert(pos, static_cast<size_type>(first), static_cast<value_type>(last));
		}

		void resize( size_type count, T value = T() ) {
			if (count < this->_size) {
				for (size_type i = count; i < this->size(); i++)
					this->A.destroy(this->data + i - 1);
//					ft::allocator_traits<allocator_type>::destroy(this->A, this->data + i - 1);
				this->_size = count;
			}
			else if (count > this->_size)
			{
				if (count > _capacity)
					this->reserve(count);
				for (size_type j = _size; j < (long int)count; j++)
					this->A.construct(this->data + j, value);
//					ft::allocator_traits<allocator_type>::construct(this->A, this->data + j, value);
				this->_size = count;
			}
		}

		void clear() {
			for (size_type i = 0; i < this->size(); i++)
				this->A.destroy(this->data + i);
//				ft::allocator_traits<allocator_type>::destroy(this->A, this->data + i);
			this->_size = 0;
		}

		iterator erase(iterator pos) {
			iterator ret = pos;
			this->A.destroy(pos.operator->());
//			ft::allocator_traits<allocator_type>::destroy(this->A, pos.operator->());
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
			for (iterator i = first; i != last; i++)
				this->A.destroy(i.operator->());
//				ft::allocator_traits<allocator_type>::destroy(this->A, i.operator->());
			while (last != this->end())
			{
				*first = *last;
				last++;
				first++;
			}
			this->_size -= last - first;
			return (ret);
		}
		void push_back(const T &value) {
			if (this->_size == this->_capacity)
				this->reserve(this->_capacity == 0 ? 1 : this->_capacity * 2);
			this->A.construct(this->data + this->_size, value);
//			ft::allocator_traits<allocator_type>::construct(this->A, this->data + this->_size, value);
			this->_size++;
		}

		void pop_back() {
			this->_size--;
			this->A.destroy(this->data + this->_size);
//			ft::allocator_traits<allocator_type>::destroy(this->A, this->data + this->_size);
		}

		void swap(vector &other) {
//			if (ft::allocator_traits<allocator_type>::propagate_on_container_swap::value)
//				std::swap(this->A, other.A);
			allocator_type swap_alloc = this->A;
			this->A = other.A;
			other.A = swap_alloc;
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
			return (!(lhs == rhs));
		}
		friend bool operator<(const vector &lhs, const vector &rhs) {
			return (ft::lexicographical_compare<iterator, iterator>(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
		}
		friend bool operator<=(const vector &lhs, const vector &rhs) {
			return (!(rhs < lhs));
		}
		friend bool operator>(const vector &lhs, const vector &rhs) {
			return (rhs < lhs);
		}
		friend bool operator>=(const vector &lhs, const vector &rhs) {
			return (!(lhs < rhs));
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
namespace std {
	template< class T, class Alloc >
	void swap(ft::vector<T,Alloc> &lhs, ft::vector<T,Alloc> &rhs) {
		lhs.swap(rhs);
	}
}


#endif //FT_CONTAINERS_VECTOR_HPP
