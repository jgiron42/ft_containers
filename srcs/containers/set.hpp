#ifndef FT_CONTAINERS_MAP_HPP
#define FT_CONTAINERS_MAP_HPP
#include "../utils/pair.hpp"
#include "../utils/iterator.hpp"
#include "../utils/reverse_iterator.hpp"
#include "../utils/enable_if.hpp"
#include <cstddef>
#include "../utils/lexicographical_compare.hpp"
#include "../utils/equal.hpp"
//#include "is_class.hpp"
#include "../utils/distance.hpp"
#include <limits>
#include <stdexcept>
#include <iostream>
#include <utility>
#include <memory>

#define BLACK	1
#define RED		0
#define _FT_MAP_PARENT_REF(n) (n->p->r == n ? n->p->r : n->p->l)
#define _FT_MAP_SIB(n) (n->p->r == n ? n->p->l : n->p->r)
#define _FT_MAP_CLO(n) (n->p->r == n ? _FT_MAP_SIB(n)->r : _FT_MAP_SIB(n)->l)
#define _FT_MAP_DIS(n) (n->p->r == n ? _FT_MAP_SIB(n)->l : _FT_MAP_SIB(n)->r)


#ifdef DEBUG
	#define RED_OUTPUT "\033[0;41m"
	#define BLACK_OUTPUT "\033[0;0m"
	#define BLUE_OUTPUT "\033[0;44m"
	#define DEFAULT_OUTPUT BLUE_OUTPUT
	//#define DEFAULT_OUTPUT BLACK_OUTPUT
#endif

namespace ft {

	template < class Key,                                     // set::key_type
			class Compare = std::less<Key>,                     // set::key_compare
			class Allocator = std::allocator<Key>     // set::allocator_type
	> class set {
	public:
		typedef Key													key_type;
		typedef Key													value_type;
		typedef size_t												size_type;
		typedef Compare												key_compare;
		typedef Compare												value_compare;
		typedef Allocator											allocator_type;
		typedef typename Allocator::reference						reference;
		typedef typename Allocator::const_reference					const_reference;
		typedef typename allocator_type::pointer					pointer;
		typedef typename allocator_type::const_pointer				const_pointer;
//		typedef value_type &													reference;
//		typedef const value_type &												const_reference;
//		typedef typename std::allocator_traits<allocator_type>::pointer			pointer;
//		typedef typename std::allocator_traits<allocator_type>::const_pointer	const_pointer;
	private:
		class node;
//		typedef typename std::allocator_traits<allocator_type>::template rebind_alloc<node> node_alloc;
		typedef typename allocator_type::template rebind<node>::other node_alloc;
		class node {
		public:
			node(const value_type &value, bool color, node *parent, size_type &size, typename remove_const<const node_alloc>::type  &NA) :r(0), l(0), p(parent), _size(&size), color(color), value(value), NA(&NA) {
				(*this->_size)++;
			}
			node(const node &src) : r(src.r), l(src.l), p(src.p), _size(src._size), color(src.color), value(src.value), NA(src.NA) {
				if (this->l)
				{
					// delire sous ritaline
					node tmp(value_type(this->l->value), this->l->color, this, *_size, *NA);
					tmp.l = this->l->l;
					tmp.r = this->l->r;
					this->l = new_node(tmp);
					tmp.l = NULL;
					tmp.r = NULL;
				}
				if (this->r)
				{
					node tmp(value_type(this->r->value), this->r->color, this, *_size, *NA);
					tmp.l = this->r->l;
					tmp.r = this->r->r;
					this->r = new_node(tmp);
					tmp.l = NULL;
					tmp.r = NULL;
				}
				(*this->_size)++;
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
				(*_size)--;
				if (this->p && this->p->l == this)
					this->p->l = NULL;
				if (this->p && this->p->r == this)
					this->p->r = NULL;
			}
			node *new_node(node &src)
			{
				node *ret = NA->allocate(1);
				this->NA->construct( ret, src);
				return(ret);
			}
			void delete_node(node &n)
			{
				this->NA->destroy( &n);
				NA->deallocate(&n, 1);
			}
			node			*r;
			node			*l;
			node			*p;
			size_type 		*_size;
			bool			color;
			value_type		value;
			typename remove_const<const node_alloc>::type		*NA;
		};
		template <typename U>
		class iteratorT {
		public:
			typedef U								value_type;
			typedef U&								reference;
			typedef U*								pointer;
			typedef std::ptrdiff_t 						difference_type;
			typedef std::bidirectional_iterator_tag iterator_category;
			node *pos;
			iteratorT() : pos(NULL) {};
			iteratorT(node* n) : pos(n){};
			iteratorT(const node* n) : pos((node *)n){};
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
			template <typename V>
			operator iteratorT<V>() const
			{
				return (iteratorT<V>(this->pos));
			}
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
		explicit set (const key_compare& comp = key_compare(),
					  const allocator_type& alloc = allocator_type()) : A(alloc), tree(0), _past_the_end(value_type(),0, 0, this->_size, NA) , _comp(comp), _size(0), NA(this->A), first(NULL), last(NULL) {};
		template <class InputIterator>
		set (InputIterator first, InputIterator last,
			 const key_compare& comp = key_compare(),
			 const allocator_type& alloc = allocator_type()) : A(allocator_type(alloc)), tree(0), _past_the_end(value_type(),0, 0, this->_size, NA), _comp(comp), _size(0), NA(this->A), first(NULL), last(NULL)
		{
				 this->insert(first, last);
		};
//		set (const set& x) : A(std::allocator_traits<allocator_type>::select_on_container_copy_construction(x.A)), _past_the_end(value_type(), 0, 0, this->_size, NA), _comp(x._comp), _size(0), NA(std::allocator_traits<node_alloc>::select_on_container_copy_construction(x.NA)), first(NULL), last(NULL)
		set (const set& x) : A(x.A), _past_the_end(value_type(), 0, 0, this->_size, NA), _comp(x._comp), _size(0), NA(x.NA), first(NULL), last(NULL)
		{
			if (x.tree) {
				node tmp_node(value_type(x.tree->value), x.tree->color, &_past_the_end, this->_size, this->NA);
				tmp_node.l = x.tree->l;
				tmp_node.r = x.tree->r;
				this->tree = new_node(tmp_node);
				tmp_node.l = NULL;
				tmp_node.r = NULL;
				for (node *i = this->tree; i; i = i->l)
					this->first = i;
				for (node *i = this->tree; i; i = i->r)
					this->last = i;
				_past_the_end.NA = &this->NA;
				this->tree->p = &_past_the_end;
			}
			else
			{
				this->first = NULL;
				this->last = NULL;
				this->tree = NULL;
			}
			_past_the_end.l = this->tree;
			_past_the_end.r = this->tree;
		};
		~set(){this->clear();}
		set& operator= (const set& x) {
			this->clear();
//			this->_comp = x._comp;
//			if (std::allocator_traits<node_alloc>::propagate_on_container_copy_assignment::value)
//				this->NA = std::allocator_traits<node_alloc>::select_on_container_copy_construction(x.NA);
				this->NA = x.NA;
//			if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value)
//				this->A = std::allocator_traits<allocator_type>::select_on_container_copy_construction(x.A);
				this->A = x.A;
				if (x.tree) {
					node tmp_node(value_type(x.tree->value), x.tree->color, &_past_the_end, this->_size, NA);
					tmp_node.l = x.tree->l;
					tmp_node.r = x.tree->r;
					this->tree = new_node(tmp_node);
					tmp_node.l = NULL;
					tmp_node.r = NULL;
					for (node *i = this->tree; i; i = i->l)
						this->first = i;
					for (node *i = this->tree; i; i = i->r)
						this->last = i;
					this->tree->p = &_past_the_end;
				}
				else
				{
					this->first = NULL;
					this->last = NULL;
					this->tree = NULL;
				}
			_past_the_end.l = this->tree;
			_past_the_end.r = this->tree;
			return (*this);
		}
		allocator_type get_allocator() const {return(this->A);};

//		T& at( const Key& key ) {
//			iterator ret = search(this->tree, key);
//			if (ret != this->end())
//				return (ret->second);
//			throw std::out_of_range("set::at");
//		}
//		const T& at( const Key& key ) const {
//			iterator ret = search(this->tree, key);
//			if (ret != this->end())
//				return (ret->second);
//			throw std::out_of_range("set::at");
//		}

		iterator begin() {return (iterator(this->first ? this->first : &_past_the_end));}
		const_iterator begin() const {return (const_iterator(this->first ? this->first : &_past_the_end));}
		iterator end() {return (iterator(&this->_past_the_end));}
		const_iterator end() const{return (const_iterator(&this->_past_the_end));}
		reverse_iterator rbegin() {return (reverse_iterator(this->end()));}
		const_reverse_iterator rbegin() const {return (const_reverse_iterator(this->end()));}
		reverse_iterator rend() {return (reverse_iterator(this->begin()));}
		const_reverse_iterator rend() const{return (const_reverse_iterator(this->begin()));}

		bool empty() const {
			return (!this->_size);}
		size_type size() const {return (this->_size);}
//		size_type max_size() const {return(std::allocator_traits<allocator_type>::max_size(this->NA));}
		size_type max_size() const {return(this->NA.max_size());}

		void clear() {
			if (tree)
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
			if (next != this->end() && _comp(*hint, value) && _comp(value, *next))
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

		void erase( iterator pos )
		{
//			std::cout << "erase " << pos.pos->value.first << std::endl;
//			node *tmp;
			node *n = pos.pos;
			if (this->first == n)
				this->first = (++iterator(n)).pos;
			if (this->last == n)
				this->last = (--iterator(n)).pos;
			if (pos.pos == this->tree && !n->r && !n->l)
			{
				delete_node(*pos.pos);
				this->tree = NULL;
				this->_past_the_end.l = NULL;
				this->_past_the_end.r = NULL;
				return;
			}
			if (n->l && n->r)
				swap_node(n, (++iterator(pos)).pos);
			if (n->color == RED)
			{
				_FT_MAP_PARENT_REF(n) = NULL;
				delete_node(*pos.pos);
				return;
			}
			if (n->l && n->l->color == RED)
			{
				n->l->color = BLACK;
				this->transplant(n, n->l);
				n->l = NULL;
				delete_node(*pos.pos);
				return;
			}
			if (n->r && n->r->color == RED)
			{
				n->r->color = BLACK;
				this->transplant(n, n->r);
				n->r = NULL;
				delete_node(*pos.pos);
				return;
			}
			node *s;
			while (n != this->tree && n->color == BLACK)
			{
//				this->print();
				if (n == n->p->l) {
					s = n->p->r;
					if (s->color == RED) {
						s->color = BLACK;
						n->p->color = RED;
						rotate_right(n->p);
						s = n->p->r;
					}
					if ((!s->l || s->l->color == BLACK) && (!s->r || s->r->color == BLACK)) {
						s->color = RED;
						n = n->p;
					} else {
						if (!s->r || s->r->color == BLACK) {
							s->l->color = BLACK;
							s->color = RED;
							rotate_left(s);
							s = n->p->r;
						}
						s->color = n->p->color;
						n->p->color = BLACK;
						if (s->r)
							s->r->color = BLACK;
//						if (s->color == BLACK)
							rotate_right(n->p);
						n = this->tree;
					}
				} else {
					s = n->p->l;
					if (s->color == RED) {
						s->color = BLACK;
						n->p->color = RED;
						rotate_left(n->p);
//						this->print();
						s = n->p->l;
					}
					if ((!s->l || s->l->color == BLACK) && (!s->r || s->r->color == BLACK)) {
						s->color = RED;
						n = n->p;
					} else {
						if (!s->l || s->l->color == BLACK) {
							s->r->color = BLACK;
							s->color = RED;
							rotate_right(s);
							s = n->p->l;
						}
						s->color = n->p->color;
						n->p->color = BLACK;
						if (s->l)
							s->l->color = BLACK;
//						if (s->color == BLACK)
							rotate_left(n->p);
						n = this->tree;
					}
				}
			}
			n->color = BLACK;
			_FT_MAP_PARENT_REF(pos.pos) = NULL;
			delete_node(*pos.pos);
			return;
		}

		void erase( iterator first, iterator last )
		{
			iterator	tmp;
			while (first != last)
			{
				tmp = first;
				first++;
				erase(tmp);
			}
		}

		size_type erase( const Key& key )
		{
			iterator tmp = find(key);
			if (tmp == end())
				return (0);
			erase(tmp);
			return (1);
		}

		void swap( set& other ) {

//			if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value)
//				std::swap(this->NA, other.NA);
//			if (std::allocator_traits<allocator_type>::propagate_on_container_swap::value)
//				std::swap(this->A, other.A);

			node_alloc NAswap = this->NA;
			this->NA = other.NA;
			other.NA = NAswap;

			allocator_type Aswap = this->A;
			this->A = other.A;
			other.A = Aswap;

			size_type	swap_size = this->_size;
			this->_size = other._size;
			other._size = swap_size;

			node		*tree_swap = this->tree;
			this->tree = other.tree;
			set_node(this->tree, (size_type *)&this->_size, (node_alloc *)&this->tree->NA);
			other.tree = tree_swap;
			set_node(other.tree, (size_type *)&other._size, (node_alloc *)&other.NA);

			other._past_the_end.l = other.tree;
			other._past_the_end.r = other.tree;
			if (other.tree)
				other.tree->p = &other._past_the_end;

			this->_past_the_end.l = this->tree;
			this->_past_the_end.r = this->tree;
			if (this->tree)
				this->tree->p = &this->_past_the_end;

			node		*first_swap = this->first;
			if (other.first != &other._past_the_end)
				this->first = other.first;
			else
				this->first = &this->_past_the_end;
			if (first_swap != &this->_past_the_end)
				other.first = first_swap;
			else
				other.first = &other._past_the_end;

			node		*last_swap = this->last;
			if (other.last != &other._past_the_end)
				this->last = other.last;
			else
				this->last = &this->_past_the_end;
			if (last_swap != &this->_past_the_end)
				other.last = last_swap;
			else
				other.last = &other._past_the_end;
		};

		size_type count( const Key& key ) const {
			return (search(this->tree, key) != this->end());
		}

		iterator find( const Key& key ) {
			return (search(this->tree, key));
		}

		const_iterator find( const Key& key ) const {
			return (search(this->tree, key));
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
			iterator ret(recursive_lower_bound(this->tree, key, (node *)&this->_past_the_end));

			if (!this->_comp(*ret, key) && !this->_comp(key, *ret))
				return (++ret);
			return (ret);
		}

		const_iterator upper_bound( const Key& key ) const {
			iterator ret(recursive_lower_bound(this->tree, key, (node *)&this->_past_the_end));

			if (!this->_comp(*ret, key) && !this->_comp(key, *ret))
				return (++ret);
			return (ret);
		}
		key_compare key_comp() const {
			return(this->_comp);
		}

		value_compare value_comp() const {
			return (value_compare(this->_comp));
		};

	private:
		void transplant(node *a, node *b)
		{
			if (a == a->p->r)
				a->p->r = b;
			if (a == a->p->l)
				a->p->l = b;
			if (b)
				b->p = a->p;
			if (a == this->tree)
				this->tree = b;
		}
		void 	swap_node(node *a, node *b)
		{
			node *swap_node;

			if (a->r)
				a->r->p = b;
			if (a->l)
				a->l->p = b;
			if (b->r)
				b->r->p = a;
			if (b->l)
				b->l->p = a;
			swap_node = a->r;
			a->r = b->r;
			b->r = swap_node;

			swap_node = a->l;
			a->l = b->l;
			b->l = swap_node;

			if (a->p->r == a)
				a->p->r = b;
			if (a->p->l == a)
				a->p->l = b;
			if (b->p->r == b)
				b->p->r = a;
			if (b->p->l == b)
				b->p->l = a;
			swap_node = a->p;
			a->p = b->p;
			b->p = swap_node;
			bool swap_color = a->color;
			a->color = b->color;
			b->color = swap_color;
			if (this->tree == a)
				this->tree = b;
			else if (this->tree == b)
				this->tree = a;
//			if (this->first == a)
//				this->first = b;
//			else if (this->first == b)
//				this->first = a;
//
//			if (this->last == a)
//				this->last = b;
//			else if (this->last == b)
//				this->last = a;

		}
		void 	invert_color(node *a)
		{
			if (!a)
				return;
			if (a)
				a->color ^= 1;
			if (a->r)
				invert_color(a->r);
			if (a->l)
				invert_color(a->l);
		}
		void	set_node(node *n, size_type *size, node_alloc * NA)
		{
			if (!n)
				return;
			if( n->l)
				set_node(n->l, size, NA);
			if( n->r)
				set_node(n->r, size, NA);
			if (size)
				n->_size = size;
			if (NA)
				n->NA = NA;
		}
		node *new_node(node &n)
		{
			node *ret = NA.allocate(1);
			this->NA.construct( ret, n);
			return(ret);
		}
		void delete_node(node &n)
		{
			this->NA.destroy( &n);
			NA.deallocate(&n, 1);
		}

	 	iterator recursive_lower_bound(node *n, const Key &k, node *current) const
		{
			if (!n)
				return(iterator(current));
			if ((current == &_past_the_end || _comp(n->value, current->value)) && !_comp(n->value, k))
				current = n;
			if (_comp(k, n->value))
				return(recursive_lower_bound(n->l, k, current));
			if (_comp(n->value, k))
				return(recursive_lower_bound(n->r, k, current));
			return (iterator(n));
		}

		iterator recursive_upper_bound(node *n, const Key &k, node *current) const
		{
			if (!n)
				return(iterator(current));
			if ((current == &_past_the_end || _comp(n->value, current->value)) && _comp(k, n->value))
				current = n;
			if (_comp(k, n->value))
				return(recursive_upper_bound(n->l, k, current));
			if (_comp(n->value, k))
				return(recursive_upper_bound(n->r, k, current));
			return (iterator(current));
		}

	 	iterator search(node *n, const Key &k) const
		{
			while (1) {
				if (!n)
					return(iterator(&this->_past_the_end));
				else if (_comp(k, n->value))
					n = n->l;
				else if (_comp(n->value, k))
					n = n->r;
				else
					return (iterator(n));
			}
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
			if (_comp(n->value, value))						// greater
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
			else if (_comp(value, n->value))					// smaller
			{
				if (n->l)
					return(recursive_insert(value, n->l));
				else {
					tmpnode = node(value, RED, n, this->_size, NA);
					n->l = new_node(tmpnode);
					ret = n->l;
					if (n == first)
						first = n->l;
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
		node *get_uncle(node *n) {
			if (!exist(n->p))
				return (NULL);
			if (!exist(n->p->p))
				return (NULL);
			if (n->p->p->l == n->p)
				return n->p->p->r;
			else
				return n->p->p->l;
		}
	public:
		friend bool operator==(const set &lhs, const set &rhs) {
			return (ft::equal<const_iterator, const_iterator>(lhs.begin(), lhs.end(), rhs.begin(),	rhs.end()));
		}
		friend bool operator!=(const set &lhs, const set &rhs) {
			return (!(lhs == rhs));
		}
		friend bool operator<(const set &lhs, const set &rhs) {
			return (ft::lexicographical_compare<const_iterator, const_iterator>(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
		}
		friend bool operator<=(const set &lhs, const set &rhs) {
			return (!(rhs < lhs));
		}
		friend bool operator>(const set &lhs, const set &rhs) {
			return (rhs < lhs);
		}
		friend bool operator>=(const set &lhs, const set &rhs) {
			return (!(lhs < rhs));
		}

#ifdef DEBUG
	public:
		bool print() {
			char tmp[1000] = {};
			if (this->tree)
				print_node(this->tree, 0, tmp);
			return rb_test();
		}
		bool	rb_test()
		{
			if (is_rb_shaped(this->tree) == -1)
			{
				std::cout << "!!!!!!! NOT IN RB-SHAPE !!!!!!!!" << std::endl;
				return (0);
			}
			return (1);
		}
	private:
		int is_rb_shaped(node *n)
		{
			if (!n)
				return (0);
			if (exist(n->p) && n->p->color == RED && n->color == RED)
				return (-1);
			int r1 = is_rb_shaped(n->l);
			int r2 = is_rb_shaped(n->r);
			if (r1 == -1 || r2 == -1 || r1 != r2)
				return (-1);
			return (r1 + (n->color == BLACK));
		}
		void print_node(node* n, int depth, char *tmp)  {
			if (n->l)
				print_node(n->l, depth + 1, tmp);
			for  (int i = 0; i < depth; i++)
			{
				if (i + 1 == depth)
				{
					if (n->p->l == n)
					{
						std::cout << "    ???";
						tmp[i] = 1;
					}
					else
					{
						std::cout << "    ???";
						tmp[i]  = 0;
					}
				}
				else
				{
					if (tmp[i])
						std::cout << "    ???";
					else
						std::cout << "     ";
				}
			}
			if (n->color == RED)
				std::cout << RED_OUTPUT;
			if (n->color == BLACK)
				std::cout << BLACK_OUTPUT;
			std::cout << n->value << DEFAULT_OUTPUT;
			if (n->value < 10 && (n->l || n->r))
				std::cout << "???";
			if (n->value < 100 && (n->l || n->r))
				std::cout << "???";
			if (n->value < 1000 && (n->l || n->r))
				std::cout << "???";
			if (n->r && n->l)
			{
				std::cout << "???";
				tmp[depth] = 1;
			}
			if (n->l && !n->r)
			{
				std::cout << "???";
				tmp[depth] = 0;
			}
			if (n->r && !n->l)
			{
				tmp[depth] = 1;
				std::cout << "???";
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
	template<class Key, class Compare, class Alloc>
	void swap(ft::set<Key, Compare, Alloc> &lhs, ft::set<Key, Compare, Alloc> &rhs) {
		lhs.swap(rhs);
	}
}

#endif
