#ifndef FT_CONTAINERS_MAP_HPP
#define FT_CONTAINERS_MAP_HPP
#include "pair.hpp"
#include "iterator.hpp"
#include "reverse_iterator.hpp"
#include "enable_if.hpp"
#include "is_class.hpp"
#include <limits>
#include <stdexcept>
#define BLACK	1
#define RED		0
#define RIGHT	1
#define LEFT	0

namespace ft {
	template < class Key,                                     // map::key_type
			class T,                                       // map::mapped_type
			class Compare = std::less<Key>,                     // map::key_compare
			class Alloc = std::allocator<pair<const Key,T> >    // map::allocator_type
	> class map {
	public:
		typedef Key key_type;
		typedef T mapped_type;
		typedef pair<const key_type, mapped_type> value_type;
		typedef Compare key_compare;
//		typedef value_compare??????
		typedef Alloc allocator_type;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef size_t size_type;
	private:
		class node {

		public:
			node(const value_type &value, bool color, node *parent, allocator_type &A, size_type &size) :r(0), l(0), p(parent), _size(size), A(A),  color(color), value(A.allocate(1)) {
				A.construct(this->value, value_type(value));
			}
			node(node &src) : r(src.r), l(src.l), p(src.p), _size(src._size), A(src.A), color(src.color), value(A.allocate(1)) {
				A.construct(value, value_type(src.value));
				if (this->l)
					this->l = new node(this->l);
				if (this->r)
					this->r = new node(this->r);
				(*this->_size)++;
			}
			node &operator=(node &src) {
				this->p = src.p;
				this->r = src.r;
				this->l = src.l;
				this->color = src.color;
				this->value = src.value;
				return (*this);
			}
			~node() {
				if (this->r)
					delete this->r;
				if (this->l)
					delete this->l;
				A.deallocate(this->value, 1);
				_size--;
				if (this->p && this->p->l == this)
					this->p->l = NULL;
				if (this->p && this->p->r == this)
					this->p->r = NULL;
			}
			node			*r;
			node			*l;
			node			*p;
			size_type 		&_size;
			allocator_type	&A;
			bool			color;
			value_type		*value;
		};
	public:
//		template <typename U>
		class iterator {
		private:
			node *pos;
		public:
			iterator(node *n) : pos(n){};
			iterator(iterator &src) : pos(src.operator->()){}
			iterator operator=(iterator &src) {this->pos = src.operator->();}
			node *operator->() {return (this->pos);}
			value_type &operator*() {return (this->pos->value);}
			iterator &operator++() {
				node *tmp = *this;
				return(*this);
			}
		private:
			iterator &increment(node *n) {
				if (n->r)
					return (this->pos = n->r);
				node *tmp = n;
				n = n->p;
				while (n && n->r == tmp)
				{
					tmp = n;
					n = n->p;
				}
				if (n)
					return (this->pos = n->r);

			}
		};
//		typedef	reverse_iterator<iterator>  reverse_iterator;
//		typedef	reverse_iterator<const_iterator> const_reverse_iterator;
//		typedef iterator_traits<iterator>::difference_type difference_type;
	private:
		allocator_type			A;
		node					*tree;
		key_compare				comp;
		size_type				_size;
	public:
		explicit map (const key_compare& comp = key_compare(),
					  const allocator_type& alloc = allocator_type()) : A(alloc), tree(0), comp(comp), _size(0) {};
//		template <class InputIterator>
//		map (InputIterator first, InputIterator last,
//			 const key_compare& comp = key_compare(),
//			 const allocator_type& alloc = allocator_type()) : A(alloc), comp(comp), tree(0), _size(0);
		map (const map& x) : A(allocator_type()), comp(key_compare()), tree(x.tree), _size(x._size) {};
		~map(){
			this->clear();
		}
		map& operator= (const map& x) {
			this->clear();
			this->A = x.A;
		}

		T& at( const Key& key ) {
			return (recursive_search(this->tree, key));
		}
		const T& at( const Key& key ) const {
			return (recursive_search(this->tree, key));
		}

//		ft::pair<iterator, bool>	insert( const value_type& value ) {
		ft::pair<node *, bool>	insert( const value_type& value ) {
			if (!this->tree)
			{
				this->tree = new node(value, BLACK, NULL, this->A, this->_size);
				return (make_pair(this->tree, true));
			}
			return (recursive_insert(value, this->tree));
		}
//		iterator					insert( iterator hint, const value_type& value );
//		template< class InputIt >
//		void						insert( InputIt first, InputIt last );

		bool empty() const {return (!this->_size);}
		size_type size() const {return (this->_size);}
//		size_type max_size() const {return(std::numeric_limits<difference_type>::max());}

		void clear() {
			delete tree;
			tree = NULL;
		};
		allocator_type get_allocator() const {return (this->A);}

		void print() {
			if (this->tree)
				print_node(this->tree, 0);
		}

	private:
//		friend bool operator==(typename ft::enable_if<is_class<Key>::value, Key>::type &a, Key &b) {return (!comp(a, b) && !comp(b, a));}
//		friend bool operator<(typename ft::enable_if<is_class<Key>::value, Key>::type &a, Key &b) {return (comp(a, b));}
//		friend bool operator>(typename ft::enable_if<is_class<Key>::value, Key>::type &a, Key &b) {return (comp(b, a));}
//		friend bool operator<=(typename ft::enable_if<is_class<Key>::value, Key>::type &a, Key &b) {return (!comp(b, a));}
//		friend bool operator>=(typename ft::enable_if<is_class<Key>::value, Key>::type &a, Key &b) {return (!comp(a, b));}
	 	T &recursive_search(node *n, const Key &k)
		{
			if (!n)
				throw std::out_of_range("node doesn't exist");
	 		if (k < n->value->first)
	 			return(recursive_search(n->l, k));
			if (k > n->value->first)
				return(recursive_search(n->r, k));
			return (n->value->second);
		}
		node *Rotate_left(node* P) {
			if (!P || !P->l)
				return (NULL);
			node *tmp = P->l;
			P->l = tmp->r;
			tmp->r = P;
			P = tmp;
			return (P);
		}

		void print_node(node* n, int depth) {
			if (n->l)
				print_node(n->l, depth + 1);
			for  (int i = 0; i < depth; i++)
			{
				if (i + 1 == depth)
				{
					if (n->p->l == n)
						std::cout << "   ╱";
					else
						std::cout << "   ╲";
				}
				else
					std::cout << "    ";
			}
			std::cout << n->value->first;
//			if (n->r && n->l)
//				std::cout << "⎨";
//			if (n->l)
//				std::cout << "";
//			if (n->r)
//				std::cout << "";
			std::cout  << std::endl;

			if (n->r)
				print_node(n->r, depth + 1);
		}
//		ft::pair<iterator, bool>	recursive_insert(const value_type& value, node *n)
		ft::pair<node *, bool>	recursive_insert(const value_type& value, node *n)
		{
//			node *tmp;
			if (value.first > n->value->first)						// greater
			{
				if (n->r)
					return (recursive_insert(value, n->r));
				else {
					n->r = new node(value, RED, n, this->A, this->_size);
					return (make_pair(n->r, true));
				}
			}
			else if (value.first < n->value->first)				// smaller
			{
				if (n->l)
					return (recursive_insert(value, n->l));
				else {
					n->l = new node(value, RED, n, this->A, this->_size);
					return (make_pair(n->l, true));
				}
			}
			else													// equal
				return (make_pair(n, false));
//			if (value->first < n->l->value->first)
//				return(recursive_insert(n->l, n));
//			if (value->first > n->r->value->first)
//				return(recursive_insert(n->r, n));
//			if (value->first < n->value->first)
//			{
//				tmp =  new node(value, RED, n, this->A, this->_size);
//				tmp->l
//
//			}
//			if (value->first > n->value->first)
//				return(recursive_insert(n->r, n));

		}
	};
}

#endif
