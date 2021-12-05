//
// Created by Joachim GIRON on 7/18/21.
//

#ifndef FT_CONTAINERS_VECTOR_HPP
#define FT_CONTAINERS_VECTOR_HPP

#include "srcs/utils/reverse_iterator.hpp"
#include "srcs/utils/lexicographical_compare.hpp"
#include "srcs/utils/equal.hpp"
#include "srcs/utils/iterator.hpp"
#include "srcs/utils/distance.hpp"
#include "srcs/utils/enable_if.hpp"
#include "srcs/utils/is_integral.hpp"
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
		typedef typename Allocator::reference reference;
		typedef typename Allocator::const_reference const_reference;
		typedef typename allocator_type::pointer				pointer;
		typedef typename allocator_type::const_pointer			const_pointer;
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
		pointer				_data;
		size_type _capacity;
		size_type _size;
	public:
		vector() : A(allocator_type()), _data(A.allocate(1)), _capacity(1), _size(0) {}

		explicit vector(const Allocator &alloc) : A(alloc), _data(A.allocate(1)), _capacity(1), _size(0) {}

		explicit vector(size_type count, const T &value = T(), const Allocator &alloc = Allocator())
		: A(alloc), _data(A.allocate(count)), _capacity(count), _size(count) {
			for (size_type i = 0; i < count; i++)
				this->A.construct(this->_data + i, value);
//				ft::allocator_traits<allocator_type>::construct(this->A, this->_data + i, value);
		}

		template< class InputIt >
		vector( InputIt first, typename ft::enable_if<!is_integral<InputIt>::value, InputIt>::type last, const Allocator& alloc = Allocator()) : A(alloc) {
			this->_size = ft::distance(first, last);
			this->_data = A.allocate(this->_size);
			this->_capacity = this->_size;
			int j = 0;
			for (InputIt i(first); i != last; i++)
				this->A.construct(this->_data + j++, *i);
//				ft::allocator_traits<allocator_type>::construct(this->A, this->_data + j++, *i);
		}

		template< class InputIt >
		vector( InputIt first, typename ft::enable_if<is_integral<InputIt>::value, InputIt>::type last, const Allocator& alloc = Allocator()) :
		A(alloc), _data(A.allocate(static_cast<size_type>(first))), _capacity(static_cast<size_type>(first)), _size(static_cast<size_type>(first)) {
			for (size_type i = 0; i < static_cast<size_type>(first); i++)
				this->A.construct(this->_data + i, static_cast<value_type>(last));
//				ft::allocator_traits<allocator_type>::construct(this->A, this->_data + i, static_cast<value_type>(last));
		}

//		vector(const vector &other) : A(ft::allocator_traits<allocator_type>::select_on_container_copy_construction(other.A)), _data(A.allocate(other._capacity)),
		vector(const vector &other) : A(other.A), _data(A.allocate(other._capacity)),
									  _capacity(other._capacity), _size(other._size) {
			for (size_type i = 0; i < other._size; i++)
				this->A.construct(this->_data + i, T(other._data[i]));
//				ft::allocator_traits<allocator_type>::construct(this->A, this->_data + i, T(other._data[i]));
		}
		~vector() {
			this->clear();
			this->A.deallocate(this->_data, this->_capacity);
		}
		vector &operator=(const vector &other) {
			this->clear();
			this->A.deallocate(this->_data, this->_capacity);
			this->A = other.A;
			this->_data = this->A.allocate(other._capacity);
//			if (ft::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value)
			this->_capacity = other._capacity;
			this->_size = other._size;
			for (size_type i = 0; i < other._size; i++)
				this->A.construct(this->_data + i, T(other._data[i]));
//				ft::allocator_traits<allocator_type>::construct(this->A, this->_data + i, T(other._data[i]));
			return (*this);
		}

		void assign(size_type count, const T &value) {
			this->clear();
			if (count > _capacity) {
				this->A.deallocate(this->_data, this->_capacity);
				this->_data = this->A.allocate(count);
				this->_capacity = count;
			}
			this->_size = count;
			for (size_type i = 0; i < count; i++)
				this->A.construct(this->_data + i, T(value));
//				ft::allocator_traits<allocator_type>::construct(this->A, this->_data + i, T(value));
		}

		template< class InputIt >
		void assign( InputIt first, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::type last ) {
			size_type count = ft::distance(first, last);
			this->clear();
			if (count > _capacity) {
				this->A.deallocate(this->_data, this->_capacity);
				this->_data = this->A.allocate(count);
				this->_capacity = count;
			}
			this->_size = count;
			int j = 0;
			for (InputIt i(first); i != last; i++)
				this->A.construct(this->_data + j++, *i);
//				ft::allocator_traits<allocator_type>::construct(this->A, this->_data + j++, *i);
		}

		allocator_type get_allocator() const { return allocator_type(this->A); }

		reference at(size_type pos) {
			range_check(pos);
			return (this->_data[pos]);
		}

		const_reference at(size_type pos) const {
			range_check(pos);
			return (this->_data[pos]);
		}

		reference operator[](size_type pos) {
			return (this->_data[pos]);
		}

		const_reference operator[](size_type pos) const {
			return (this->_data[pos]);
		}

		reference front() {
			return (this->_data[0]);
		}

		const_reference front() const {
			return (this->_data[0]);
		}

		reference back() {
			return (this->_data[this->_size - 1]);
		}

		const_reference back() const {
			return (this->_data[this->_size - 1]);
		}

		T* data()
		{
			return (this->_data);
		}

		const T* data() const
		{
			return (this->_data);
		}

		iterator begin() {
			return (iterator(this->_data));
		}

		const_iterator begin() const {
			return (const_iterator(this->_data));
		}

		iterator end() {
			return (iterator(this->_data + _size ));
		}

		const_iterator end() const {
			return (const_iterator(this->_data + _size));
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
			if (new_cap <= this->_capacity)
				return;
			if (new_cap > this->max_size())
				throw std::length_error("vector::reserve");
			pointer tmp = A.allocate(new_cap);
			for (size_type i = 0; i < this->_size; i++)
			{
				this->A.construct(tmp + i, this->_data[i]);
//				ft::allocator_traits<allocator_type>::construct(this->A, tmp + i, this->_data[i]);
				this->A.destroy(this->_data + i);
			}
			this->A.deallocate(this->_data, this->_capacity);
			this->_capacity = new_cap;
			this->_data = tmp;
		}

		size_type capacity() const {
			return (this->_capacity);
		}

		iterator insert( iterator pos, const T& value ) {
			size_type i = 0;
			size_type tmp = pos - this->begin();
			if (this->_size + 1 > this->_capacity)
			{
				size_type new_cap = get_size(1);
				pointer new_data = this->A.allocate(new_cap);
				for (; i < tmp; i++)
					this->A.construct(new_data + i, this->_data[i]);
				this->A.construct(new_data + i, value);
				++i;
				for (; i <= this->_size; i++)
					this->A.construct(new_data + i, this->_data[i - 1]);
				size_type new_size = _size + 1;
				this->clear();
				this->A.deallocate(this->_data, this->_capacity);
				this->_data = new_data;
				this->_size = new_size;
				this->_capacity = new_cap;
				return (iterator(this->_data + tmp));
			}
			i = this->_size;
			if (i != tmp)
			{
				this->A.construct(this->_data + i, this->_data[i - 1]);
				--i;
				for (; i > tmp; i--)
					this->_data[i] = this->_data[i - 1];
				this->_data[i] = value;
			}
			else
				this->A.construct(this->_data + i, value);
			this->_size++;
			return (pos);
		}

		void insert( iterator pos, size_type count, const T& value ) {
			if (!count)
				return;
			size_type tmp = pos - this->begin();
			size_type i = 0;

			if (this->_size + count > this->_capacity)
			{
				size_type new_cap = get_size(count);
				pointer new_data = this->A.allocate(new_cap);
				for (; i < tmp; i++)
					this->A.construct(new_data + i, this->_data[i]);
				for (size_type j = 0; j < count; j++) {
					this->A.construct(new_data + i, value);
					++i;
				}
				for (; i < this->_size + count; i++)
					this->A.construct(new_data + i, this->_data[i - count]);
				size_type new_size = _size + count;
				this->clear();
				this->A.deallocate(this->_data, this->_capacity);
				this->_data = new_data;
				this->_size = new_size;
				this->_capacity = new_cap;
				return;
			}
			i = this->_size + count;
			while (i > tmp + count)
			{
				if (i > _size)
					this->A.construct(this->_data + i - 1, this->_data[i - count - 1]);
				else
					this->_data[i - 1] = this->_data[i - count - 1];
				--i;
			}
			while (i > tmp)
			{
				if (i > _size)
					this->A.construct(this->_data + i - 1, value);
				else
					this->_data[i - 1] = value;
				--i;
			}
			this->_size += count;
		}


		template<class InputIt>
		void insert( iterator pos, typename ft::enable_if<!is_integral<InputIt>::value, InputIt>::type first, InputIt last) {
			size_type count = ft::distance(first, last);
			if (!count)
				return;
			size_type tmp = pos - this->begin();
			size_type i = 0;

			if (this->_size + count > this->_capacity)
			{
				size_type new_cap = get_size(count);
				pointer new_data = this->A.allocate(new_cap);
				for (; i < tmp; i++)
					this->A.construct(new_data + i, this->_data[i]);
				for (size_type j = 0; j < count; j++) {
					this->A.construct(new_data + i, *first++);
					++i;
				}
				for (; i < this->_size + count; i++)
					this->A.construct(new_data + i, this->_data[i - count]);
				size_type new_size = _size + count;
				this->clear();
				this->A.deallocate(this->_data, this->_capacity);
				this->_data = new_data;
				this->_size = new_size;
				this->_capacity = new_cap;
				return;
			}
			i = this->_size + count;
			while (i > tmp + count)
			{
				if (i > _size)
					this->A.construct(this->_data + i - 1, this->_data[i - count - 1]);
				else
					this->_data[i - 1] = this->_data[i - count - 1];
				--i;
			}
			while (i > tmp)
			{
				if (i > _size)
					this->A.construct(this->_data + i - 1, *--last);
				else
					this->_data[i - 1] = *--last;
				--i;
			}
			this->_size += count;
		}


		template<class InputIt>
		void insert( iterator pos, typename ft::enable_if<is_integral<InputIt>::value, InputIt>::type first, InputIt last) {
			this->insert(pos, static_cast<size_type>(first), static_cast<value_type>(last));
		}

		void resize( size_type count, T value = T() ) {
			if (count < this->_size) {
				for (size_type i = count; i < this->_size; i++)
					this->A.destroy(this->_data + i);
//					ft::allocator_traits<allocator_type>::destroy(this->A, this->_data + i);
				this->_size = count;
			}
			else if (count > this->_size)
			{
				if (count > _capacity)
					this->reserve(count);
				for (size_type j = _size; j < count; j++)
					this->A.construct(this->_data + j, T(value));
//					ft::allocator_traits<allocator_type>::construct(this->A, this->_data + j, value);
				this->_size = count;
			}
		}

		void
		clear() {
			while (this->_size > 0)
			{
				this->A.destroy(this->_data + this->_size - 1);
				//				this->A.destroy(this->_data + this->_size - 1);
				--this->_size;
			}
			this->_size = 0;
		}

		iterator erase(iterator pos) {
			iterator ret = pos;
//			this->A.destroy(_data + _size - 1);
//			ft::allocator_traits<allocator_type>::destroy(this->A, pos.operator->());
			while (pos + 1 < this->end())
			{
				pos[0] = pos[1];
				pos++;
			}
			this->A.destroy(_data + _size - 1);
			this->_size--;
			return (ret);
		}

		iterator erase( iterator first, iterator last ) {
			iterator ret = first;
			int	distance = last - first;
			while (last != this->end())
			{
				*first = *last;
				last++;
				first++;
			}
//			for (iterator i = first; i != last; i++)
			while (first != this->end())
			{
				this->A.destroy(first.operator->());
//				ft::allocator_traits<allocator_type>::destroy(this->A, first.operator->());
				++first;
			}
			this->_size -= distance;
			return (ret);
		}
		void push_back(const T &value) {
			if (this->_size == this->_capacity)
				this->reserve(get_size(1));
			this->A.construct(this->_data + this->_size, value);
//			ft::allocator_traits<allocator_type>::construct(this->A, this->_data + this->_size, value);
			this->_size++;
		}

		void pop_back() {
			this->_size--;
			this->A.destroy(this->_data + this->_size);
//			ft::allocator_traits<allocator_type>::destroy(this->A, this->_data + this->_size);
		}

		void swap(vector &other) {
//			if (ft::allocator_traits<allocator_type>::propagate_on_container_swap::value)
//				std::swap(this->A, other.A);
			allocator_type swap_alloc = this->A;
			this->A = other.A;
			other.A = swap_alloc;
			pointer tmpdata = this->_data;
			this->_data = other._data;
			other._data = tmpdata;
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
		size_type get_size(size_type wanted)
		{
			size_type ret = _capacity;
			if (!ret)
				ret = 1;
			while (ret < wanted + _size)
				ret *= 2;
			return (ret);
		}
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
