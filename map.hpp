#ifndef FT_CONTAINERS_MAP_HPP
#define FT_CONTAINERS_MAP_HPP
#include "pair.hpp"
#include "iterator.hpp"
#include "reverse_iterator.hpp"
#include "enable_if.hpp"
#include "type_traits"
#include <cstddef>
#include "lexicographical_compare.hpp"
#include "equal.hpp"
//#include "is_class.hpp"
#include "distance.hpp"
#include <limits>
#include <stdexcept>
#include <iostream>
#include <utility>
#define BLACK	1
#define RED		0

#define DEBUG

#ifdef DEBUG
	#define RED_OUTPUT "\033[0;41m"
	#define BLACK_OUTPUT "\033[0;0m"
	#define BLUE_OUTPUT "\033[0;44m"
	#define DEFAULT_OUTPUT BLUE_OUTPUT
	//#define DEFAULT_OUTPUT BLACK_OUTPUT
#endif

namespace ft {

	template < class Key,                                     // map::key_type
			class T,                                       // map::mapped_type
			class Compare = std::less<Key>,                     // map::key_compare
			class Alloc = std::allocator<pair<const Key,T> >    // map::allocator_type
	> class map {
	public:
		typedef Key													key_type;
		typedef T													mapped_type;
		typedef pair<const key_type, mapped_type>					value_type;
		typedef Compare												key_compare;
		class value_compare : public std::binary_function<value_type, value_type, bool>{
		protected:
			Compare comp;
		public:
			value_compare(Compare c) : comp(c) {}
			bool operator()( const value_type& lhs, const value_type& rhs ) const {
				return(comp(lhs.first, rhs.first));
			}
		};

		typedef Alloc									allocator_type;
		typedef typename allocator_type::reference					reference;
		typedef typename allocator_type::const_reference			const_reference;
		typedef typename allocator_type::pointer					pointer;
		typedef typename allocator_type::const_pointer				const_pointer;
		typedef size_t												size_type;
	private:
//		typedef typename remove_const<const allocator_type>::type	unconsted_alloc;
		class node;
		typedef typename Alloc::template rebind<node>::other node_alloc;
		class node {
		public:
			node(const value_type &value, bool color, node *parent, size_type &size, typename remove_const<const node_alloc>::type  &NA) :r(0), l(0), p(parent), _size(size), color(color), value(value), NA(NA) {
				this->_size++;
			}
			node(const node &src) : r(src.r), l(src.l), p(src.p), _size(src._size), color(src.color), value(src.value), NA(src.NA) {
				if (this->l)
					this->l = new_node(*this->l);
				if (this->r)
					this->r = new_node(*this->r);
				this->_size++;
			}
			node &operator=(const node &src) {
				this->p = src.p;
				this->r = src.r;
				this->l = src.l;
				this->color = src.color;
				this->value = value_type(src.value);
				return (*this);
			}
			~node() {
				if (this->r)
					delete_node(*this->r);
				if (this->l)
					delete_node(*this->l);
				_size--;
				if (this->p && this->p->l == this)
					this->p->l = NULL;
				if (this->p && this->p->r == this)
					this->p->r = NULL;
			}
			node *new_node(node &src)
			{
				node *ret = NA.allocate(1);
				NA.construct(ret, src);
				return(ret);
			}
			void delete_node(node &n)
			{
				NA.destroy(&n);
				NA.deallocate(&n, 1);
			}
			node			*r;
			node			*l;
			node			*p;
			size_type 		&_size;
			bool			color;
			value_type		value;
			typename remove_const<const node_alloc>::type		&NA;
		};
		template <typename U>
		class iteratorT {
		public:
			typedef U								value_type;
			typedef U&								reference;
			typedef U*								pointer;
			typedef std::ptrdiff_t 						difference_type;
			typedef std::bidirectional_iterator_tag iterator_category;
		protected:
//			typename remove_const<const node*>::type pos;
			node *pos;
		public:
			iteratorT() : pos(NULL) {};
			iteratorT(node* n) : pos(n){};
			iteratorT(const node* n) : pos((node *)n){};
//			iteratorT(typename remove_const<const node*>::type n) : pos(n){};
			iteratorT(const iteratorT<value_type> &src) : pos(src.pos){}
			iteratorT &operator=(const iteratorT &src) {
				this->pos = (node *)src.pos;
				return(*this);
			}
			operator iteratorT<const U>()
			{
				return(iteratorT<const U>(this->pos));
			}
			pointer operator->() const {return (&(this->pos->value));}
			reference operator*() const {return (this->pos->value);}
			iteratorT &operator++() {return (increment(this->pos, NULL));}
			iteratorT &operator--() {return (decrement(this->pos, NULL));}
			iteratorT operator++(int) {
				const iteratorT tmp(*this);
				increment(this->pos, NULL);
				return (tmp);
			}
			iteratorT operator--(int) {
				iteratorT tmp(*this);
				decrement(this->pos, NULL);
				return (tmp);
			}
			friend class ft::map<Key, T, Compare, Alloc>;
		private:
			iteratorT &increment(typename remove_const<const node*>::type n, typename remove_const<const node*>::type old) {
				if (n->l && old == n->p)
					return (increment(n->l, n));
				if (old == n->p)
				{
					this->pos = (node *)n;
					return (*this);
				}
				if (n->l && n->l == old)
				{
					this->pos = (node *)n;
					return (*this);
				}
				if (n->r && n->r != old)
					return (increment(n->r, n));
				if (n->p)
					return (increment(n->p, n));
				return (*this);
			}
			iteratorT &decrement(typename remove_const<const node*>::type n, typename remove_const<const node*>::type old) {
				if (n->r && old == n->p)
					return (decrement(n->r, n));
				if (old == n->p)
				{
					this->pos = (node *)n;
					return (*this);
				}
				if (n->r && n->r == old)
				{
					this->pos = (node *)n;
					return (*this);
				}
				if (n->l && n->l != old)
					return (decrement(n->l, n));
				if (n->p)
					return (decrement(n->p, n));
				return (*this);
			}
		};
	public:
		typedef iteratorT<value_type>								iterator;
		typedef iteratorT<const value_type>							const_iterator;
		typedef	ft::reverse_iterator<iterator>						reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>				const_reverse_iterator;
		typedef typename iterator_traits<iterator>::difference_type difference_type;
	private:
		allocator_type 			A;
		node					*tree;
		node					_past_the_end;
		const key_compare		&_comp;
		size_type				_size;
		node_alloc 				NA;
		node					*first;
		node					*last;
	public:
		explicit map (const key_compare& comp = key_compare(),
					  const allocator_type& alloc = allocator_type()) : A(allocator_type(alloc)), tree(0), _past_the_end(value_type(),0, 0, this->_size, NA) , _comp(comp), _size(0) {};
		template <class InputIterator>
		map (InputIterator first, InputIterator last,
			 const key_compare& comp = key_compare(),
			 const allocator_type& alloc = allocator_type()) : A(allocator_type(alloc)), tree(0), _past_the_end(value_type(),0, 0, this->_size, NA), _comp(comp), _size(0)
		{
			 for (; first != last; first++)
				 this->insert(*first);
		};
		map (const map& x) : A(allocator_type(x.A)), tree(x.tree), _past_the_end(value_type(), 0, 0, this->_size, x.NA), _size(x._size), NA(x.NA) {
			_past_the_end.l = this->tree;
			_past_the_end.r = this->tree;
			this->tree->p = &_past_the_end;
		};
		~map(){this->clear();}
		map& operator= (const map& x) {
			this->clear();
//			this->_comp = x._comp;
			this->tree = x.tree;
			this->_size = x._size;
			_past_the_end.l = this->tree;
			_past_the_end.r = this->tree;
			this->tree->p = &_past_the_end;
			this->A = x.A;
			return (*this);
		}
		allocator_type get_allocator() const {return(this->A);};

		T& at( const Key& key ) {
			return (recursive_search(this->tree, key)->value.second);
		}
		const T& at( const Key& key ) const {
			return (recursive_search(this->tree, key)->value.second);
		}
		T& operator[]( const Key& key ){
			return (insert(ft::make_pair(key, T())).first->second);
		}

		iterator begin() {return (iterator(this->first));}
		const_iterator begin() const {return (const_iterator(this->first));}
		iterator end() {return (iterator(&this->_past_the_end));}
		const_iterator end() const{return (const_iterator(&this->_past_the_end));}
		reverse_iterator rbegin() {return (reverse_iterator(this->last));}
		const_reverse_iterator rbegin() const {return (const_reverse_iterator(this->last));}
		reverse_iterator rend() {return (reverse_iterator(&this->_past_the_end));}
		const_reverse_iterator rend() const{return (const_reverse_iterator(&this->_past_the_end));}

		bool empty() const {return (!this->_size);}
		size_type size() const {return (this->_size);}
		size_type max_size() const {return(std::numeric_limits<difference_type>::max());}

		void clear() {
			delete_node(*tree);
			tree = NULL;
			first = NULL;
			last = NULL;
			this->_past_the_end.l = NULL;
			this->_past_the_end.r = NULL;
		};

		ft::pair<iterator, bool>	insert( const value_type& value ) {
			if (!this->tree)
			{
				node n(value, RED, &this->_past_the_end, this->_size, NA);
				this->tree = new_node(n);
				this->_past_the_end.l = this->tree;
				this->_past_the_end.r = this->tree;
				this->tree->p = &this->_past_the_end;
				this->first = this->tree;
				this->last = this->tree;

				return(ft::make_pair<iterator, bool>(iterator(this->tree), true));
			}
			return (recursive_insert(value, this->tree));
		}
		iterator					insert( iterator hint, const value_type& value )
		{
			if (!this->tree)
			{
				node n(value, RED, &this->_past_the_end, this->_size, NA);
				this->tree = new_node(n);
				this->_past_the_end.l = this->tree;
				this->_past_the_end.r = this->tree;
				this->tree->p = &this->_past_the_end;
				this->first = this->tree;
				this->last = this->tree;
				return(iterator(this->tree));
			}
			iterator next = hint;
			++next;
			if (next != this->end() && _comp(hint->first, value.first) && _comp(value.first, next->first))
				return (recursive_insert(value, next.pos).first);
			return (recursive_insert(value, this->tree).first);
		}

		template< class InputIt >
		void						insert( InputIt first, InputIt last )
		{
			iterator tmp = this->begin();

			while (first != last)
			{
				tmp = this->insert(tmp, *first);
				++first;
			}
		}

//		void erase( iterator pos );
//		void erase( iterator first, iterator last );
//		size_type erase( const key_type& key );

		void swap( map& other ) {
			node_alloc	swap_alloc = this->NA;
			this->NA = other.NA;
			other.NA = swap_alloc;

			size_type	swap_size = this->_size;
			this->_size = other._size;
			other._size = swap_size;

			node		*tree_swap = this->tree;
			this->tree = other.tree;
			other.tree = tree_swap;

			other._past_the_end.l = other.tree;
			other._past_the_end.r = other.tree;
			other.tree->p = &other._past_the_end;

			this->_past_the_end.l = other.tree;
			this->_past_the_end.r = other.tree;
			this->tree->p = &this->_past_the_end;

			node		*first_swap = this->first;
			this->first = other.first;
			other.first = first_swap;

			node		*last_swap = this->last;
			this->last = other.last;
			other.last = last_swap;
		};

		size_type count( const Key& key ) const {
			return (recursive_search(this->tree, key) != this->end());
		}

		iterator find( const Key& key ) {
			return (recursive_search(this->tree, key));
		}

		const_iterator find( const Key& key ) const {
			return (recursive_search(this->tree, key));
		}

		ft::pair<iterator,iterator> equal_range( const Key& key ) {
			return (ft::make_pair(this->lower_bound(key), this->upper_bound(key)));
		}

		ft::pair<const_iterator,const_iterator> equal_range( const Key& key ) const {
			return (ft::make_pair(const_iterator(this->lower_bound(key)), const_iterator(this->upper_bound(key))));
		}

		iterator lower_bound( const Key& key ) {
			return (recursive_lower_bound(this->tree, key, (node *)&this->_past_the_end));
		}

		const_iterator lower_bound( const Key& key ) const {
			return (recursive_lower_bound(this->tree, key, (node *)&this->_past_the_end));
		}

		iterator upper_bound( const Key& key ) {
			return (recursive_upper_bound(this->tree, key, (node *)&this->_past_the_end));
		}

		const_iterator upper_bound( const Key& key ) const {
			return (recursive_upper_bound(this->tree, key, (node *)&this->_past_the_end));
		}
		key_compare key_comp() const {
			return(this->_comp);
		}

		value_compare value_comp() const {
			return (value_compare(this->_comp));
		};

	private:
		node *new_node(node &n)
		{
			node *ret = NA.allocate(1);
			NA.construct(ret, n);
			return(ret);
		}
		void delete_node(node &n)
		{
			NA.destroy(&n);
			NA.deallocate(&n, 1);
		}

	 	iterator recursive_lower_bound(node *n, const Key &k, node *current) const
		{
			if (!n)
				return(iterator(current));
			if ((current == &_past_the_end || _comp(n->value.first, current->value.first)) && !_comp(n->value.first, k))
				current = n;
			if (_comp(k, n->value.first))
				return(recursive_lower_bound(n->l, k, current));
			if (_comp(n->value.first, k))
				return(recursive_lower_bound(n->r, k, current));
			return (iterator(n));
		}

		iterator recursive_upper_bound(node *n, const Key &k, node *current) const
		{
			if (!n)
				return(iterator(current));
			if ((current == &_past_the_end || _comp(n->value.first, current->value.first)) && _comp(k, n->value.first))
				current = n;
			if (_comp(k, n->value.first))
				return(recursive_upper_bound(n->l, k, current));
			if (_comp(n->value.first, k))
				return(recursive_upper_bound(n->r, k, current));
			return (iterator(current));
		}

	 	iterator recursive_search(node *n, const Key &k) const
		{
			if (!n)
				return(iterator(&this->_past_the_end));
	 		if (_comp(k, n->value.first))
	 			return(recursive_search(n->l, k));
			if (_comp(n->value.first, k))
				return(recursive_search(n->r, k));
			return (iterator(n));
		}
		node *rotate_left(node* P) {
			node **ref = exist(P->p) && P->p->l == P ? &(P->p->l) : (exist(P->p) ? &(P->p->r) : NULL);
			node *tmp = P->l;
			tmp->p = P->p;
			P->l = tmp->r;
			if (exist(tmp->r))
				tmp->r->p = P;
			tmp->r = P;
			P->p = tmp;
			if (!ref)
			{
				this->tree = tmp;
				this->tree->p = &this->_past_the_end;
				this->_past_the_end.l = this->tree;
				this->_past_the_end.r = this->tree;
			}
			else
				*ref = tmp;
			return (P);
		}

		node *rotate_right(node* P) {
			node **ref = exist(P->p) && P->p->l == P ? &P->p->l : (exist(P->p) ? &P->p->r : NULL);
			node *tmp = P->r;
			tmp->p = P->p;
			P->r = tmp->l;
			if (exist(tmp->l))
				tmp->l->p = P;
			tmp->l = P;
			P->p = tmp;
			if (!ref)
			{
				this->tree = tmp;
				this->tree->p = &this->_past_the_end;
				this->_past_the_end.l = this->tree;
				this->_past_the_end.r = this->tree;
			}
			else
				*ref = tmp;
			return (P);
		}
		bool exist (node *n)
		{
			return (n && n != &this->_past_the_end);
		}

		ft::pair<iterator, bool>	recursive_insert(const value_type& value, node *n)
		{
			node *ret;
			node *uncle;
			node tmpnode(value_type(),0,0, this->_size, this->NA);
			if (_comp(n->value.first, value.first))						// greater
			{
				if (n->r)
					return(recursive_insert(value, n->r));
				else {
					tmpnode = node(value, RED, n, this->_size, NA);
					n->r = new_node(tmpnode);
					ret = n->r;
					if (n == last)
						last = n->r;
				}
			}
			else if (_comp(value.first, n->value.first))					// smaller
			{
				if (n->l)
					return(recursive_insert(value, n->l));
				else {
					tmpnode = node(value, RED, n, this->_size, NA);
					n->l = new_node(tmpnode);
					ret = n->l;
					if (n == first)
						first = n->r;
				}
			}
			else													// equal
				return (ft::make_pair<iterator, bool>(iterator(n), false));
			node *tmp = ret;
			start_of_balancing:
			if (tmp->p->color == BLACK)
				return (ft::make_pair<iterator, bool>(iterator(ret), true));
			if (!exist(tmp->p->p)) // parent is root
			{
				tmp->p->color = BLACK;
				return (ft::make_pair<iterator, bool>(iterator(ret), true));
			}
			uncle = get_uncle(tmp);
			if (exist(uncle) && uncle->color == RED) {
				uncle = get_uncle(tmp);
				tmp->p->color = BLACK;
				uncle->color = BLACK;
				tmp->p->p->color = RED;
				tmp = tmp->p->p;
				if (exist(tmp) && exist(tmp->p))
					goto start_of_balancing;
				return(ft::make_pair<iterator, bool>(iterator(ret), true));
			}

			if (tmp == tmp->p->l) {
				if (tmp->p->p->r == tmp->p) {
					rotate_left(tmp->p);
					tmp->color = BLACK;
					tmp->p->color = RED;
					rotate_right(tmp->p);
				}
				else
				{
					tmp->p->color = BLACK;
					tmp->p->p->color = RED;
					rotate_left(tmp->p->p);
				}
			}
			else {
				if (tmp->p->p->l == tmp->p) {
					rotate_right(tmp->p);
					tmp->color = BLACK;
					tmp->p->color = RED;
					rotate_left(tmp->p);
				}
				else
				{
					tmp->p->color = BLACK;
					tmp->p->p->color = RED;
					rotate_right(tmp->p->p);
				}
			}
			return(ft::make_pair<iterator, bool>(iterator(ret), true));
		}
		node *get_uncle(node *n){

			if (!exist(n->p))
				return (NULL);
			if (!exist(n->p->p))
				return (NULL);
			if (n->p->p->l == n->p)
				return n->p->p->r;
			else
				return n->p->p->l;
		}
		friend bool operator==(const map &lhs, const map &rhs) {
			return (ft::equal<const_iterator, const_iterator>(lhs.begin(), lhs.end(), rhs.begin(),	rhs.end()));
		}
		friend bool operator!=(const map &lhs, const map &rhs) {
			return (!(lhs == rhs));
		}
		friend bool operator<(const map &lhs, const map &rhs) {
			return (ft::lexicographical_compare<const_iterator, const_iterator>(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
		}
		friend bool operator<=(const map &lhs, const map &rhs) {
			return (!(rhs < lhs));
		}
		friend bool operator>(const map &lhs, const map &rhs) {
			return (rhs < lhs);
		}
		friend bool operator>=(const map &lhs, const map &rhs) {
			return (!(lhs < rhs));
		}

#ifdef DEBUG
	public:
		void print() {
			char tmp[1000] = {};
			if (this->tree)
				print_node(this->tree, 0, tmp);
		}
		void test_rotate_left (){
			this->tree = this->rotate_left(this->tree);
		};
	private:
		void print_node(node* n, int depth, char *tmp)  {
			if (n->l)
				print_node(n->l, depth + 1, tmp);
			for  (int i = 0; i < depth; i++)
			{
				if (i + 1 == depth)
				{
					if (n->p->l == n)
					{
						std::cout << "    ╔";
						tmp[i] = 1;
					}
					else
					{
						std::cout << "    ╚";
						tmp[i]  = 0;
					}
				}
				else
				{
					if (tmp[i])
						std::cout << "    ║";
					else
						std::cout << "     ";
				}
			}
			if (n->color == RED)
				std::cout << RED_OUTPUT;
			if (n->color == BLACK)
				std::cout << BLACK_OUTPUT;
			std::cout << n->value.first << DEFAULT_OUTPUT;
			if (n->value.first < 10 && (n->l || n->r))
				std::cout << "═";
			if (n->value.first < 100 && (n->l || n->r))
				std::cout << "═";
			if (n->value.first < 1000 && (n->l || n->r))
				std::cout << "═";
			if (n->r && n->l)
			{
				std::cout << "╣";
				tmp[depth] = 1;
			}
			if (n->l && !n->r)
			{
				std::cout << "╝";
				tmp[depth] = 0;
			}
			if (n->r && !n->l)
			{
				tmp[depth] = 1;
				std::cout << "╗";
			}
			std::cout  << std::endl;

			if (n->r)
				print_node(n->r, depth + 1, tmp);
		}
#endif
	};
	template <typename i1, typename i2>
	bool operator==(i1 l, i2 r){return (&*l == &*r );}
	template <typename i1, typename i2>
	bool operator!=(i1 l, i2 r){return !(&*l == &*r );}

}


namespace std {
	template<class Key, class T, class Compare, class Alloc>
	void swap(ft::map<Key, T, Compare, Alloc> &lhs, ft::map<Key, T, Compare, Alloc> &rhs) {
		lhs.swap(rhs);
	}
}

#endif
