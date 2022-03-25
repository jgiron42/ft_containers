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
#include "RBtree.hpp"

//#define BLACK	1
//#define RED		0
//#define _FT_MAP_PARENT_REF(n) (n->p->r == n ? n->p->r : n->p->l)
//#define _FT_MAP_SIB(n) (n->p->r == n ? n->p->l : n->p->r)
//#define _FT_MAP_CLO(n) (n->p->r == n ? _FT_MAP_SIB(n)->r : _FT_MAP_SIB(n)->l)
//#define _FT_MAP_DIS(n) (n->p->r == n ? _FT_MAP_SIB(n)->l : _FT_MAP_SIB(n)->r)


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
			class Allocator = std::allocator<pair<const Key,T> >    // map::allocator_type
	> class map {
	public:
		typedef Key													key_type;
		typedef T													mapped_type;
		typedef pair<const key_type, mapped_type>					value_type;
		typedef Compare												key_compare;
		typedef size_t												size_type;
		class value_compare : public std::binary_function<value_type, value_type, bool>{
		protected:
			Compare comp;
		public:
			value_compare(Compare c) : comp(c) {}
			bool operator()( const value_type& lhs, const value_type& rhs ) const {
				return(comp(lhs.first, rhs.first));
			}
		};

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
		typedef RBtree<value_type, value_compare, Allocator> tree_type;
	public:
		typedef typename tree_type::iterator								iterator;
		typedef typename tree_type::const_iterator							const_iterator;
		typedef	ft::reverse_iterator<iterator>						reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>				const_reverse_iterator;
		typedef typename iterator_traits<iterator>::difference_type difference_type;
	private:
		key_compare _comp;
		RBtree<value_type, value_compare, Allocator> tree;
	public:
		explicit map (const key_compare& comp = key_compare(),
					  const allocator_type& alloc = allocator_type()) : _comp(comp), tree(comp, alloc) {};
		template <class InputIterator>
		map (InputIterator first, InputIterator last,
			 const key_compare& comp = key_compare(),
			 const allocator_type& alloc = allocator_type()) : _comp(comp), tree(comp, alloc)
		{
				 this->insert(first, last);
		};
//		map (const map& x) : A(std::allocator_traits<allocator_type>::select_on_container_copy_construction(x.A)), _past_the_end(value_type(), 0, 0, this->size, NA), _comp(x._comp), size(0), NA(std::allocator_traits<node_alloc>::select_on_container_copy_construction(x.NA)), first(NULL), last(NULL)
		map (const map& x) : _comp(x._comp), tree(x.tree) {};
		~map(){}
		map& operator= (const map& x) {
			this->_comp = x._comp;
			this->tree = x.tree;
			return (*this);
		}
		allocator_type get_allocator() const {return(allocator_type(this->tree.allocator));};

		T& operator[]( const Key& key ){
			return (insert(ft::make_pair<key_type,  mapped_type>(key, T())).first->second);
		}

		iterator begin() {return (this->tree.begin());}
		const_iterator begin() const {return (this->tree.begin());}
		iterator end() {return (this->tree.end());}
		const_iterator end() const{return (this->tree.end());}

		reverse_iterator rbegin() {return (reverse_iterator(this->end()));}
		const_reverse_iterator rbegin() const {return (const_reverse_iterator(this->end()));}
		reverse_iterator rend() {return (reverse_iterator(this->begin()));}
		const_reverse_iterator rend() const{return (const_reverse_iterator(this->begin()));}

		bool empty() const {return (!this->tree.size);}
		size_type size() const {return (this->tree.size);}
//		size_type maxsize() const {return(std::allocator_traits<allocator_type>::maxsize(this->NA));}
		size_type max_size() const {return(this->tree.allocator.max_size());}

		void clear() {
			this->tree.clear();
		};

		ft::pair<iterator, bool>	insert( const value_type& value ) {
			return (this->tree.insert(value));
		}
		iterator					insert( iterator hint, const value_type& value )
		{
			return (this->tree.insert(hint, value).first);
		}

		template< class InputIt >
		void						insert( InputIt first, InputIt last )
		{
			iterator tmp = this->begin();

			while (first != last)
			{
				tmp = this->insert(tmp, *first);
				first++;
			}
		}

		void erase( iterator pos )
		{
			this->tree.erase(pos);
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

		size_type erase( const key_type& key )
		{
			iterator tmp = this->tree.find(ft::make_pair<key_type,  mapped_type>(key, mapped_type()));
			if (tmp == end())
				return (0);
			erase(tmp);
			return (1);
		}

		void swap( map& other ) {
			this->tree.swap(other.tree);
		};

		size_type count( const Key& key ) const {
			return (this->tree.find( ft::make_pair<key_type,  mapped_type>(key, mapped_type())) != this->end());
		}

		iterator find( const Key& key ) {
			return (this->tree.find(ft::make_pair<key_type,  mapped_type>(key, mapped_type())));
		}

		const_iterator find( const Key& key ) const {
			return (this->tree.find(ft::make_pair<key_type,  mapped_type>(key, mapped_type())));
		}

		ft::pair<iterator,iterator> equal_range( const Key& key ) {
			return (ft::make_pair<iterator, iterator>(this->tree.lower_bound(ft::make_pair<key_type,  mapped_type>(key, mapped_type())), this->tree.upper_bound(ft::make_pair<key_type,  mapped_type>(key, mapped_type()))));
		}

		ft::pair<const_iterator,const_iterator> equal_range( const Key& key ) const {
			return (ft::make_pair<const_iterator,const_iterator>(const_iterator(this->tree.lower_bound(ft::make_pair<key_type,  mapped_type>(key, mapped_type()))), const_iterator(this->tree.upper_bound(ft::make_pair<key_type,  mapped_type>(key, mapped_type())))));
		}

		iterator lower_bound( const Key& key ) {
			return (this->tree.lower_bound(ft::make_pair<key_type,  mapped_type>(key, mapped_type())));
		}

		const_iterator lower_bound( const Key& key ) const {
			return (const_iterator(this->tree.lower_bound(ft::make_pair<key_type,  mapped_type>(key, mapped_type()))));
		}

		iterator upper_bound( const Key& key ) {
			return (this->tree.upper_bound(ft::make_pair<key_type,  mapped_type>(key, mapped_type())));
		}

		const_iterator upper_bound( const Key& key ) const {
			return (const_iterator(this->tree.upper_bound(ft::make_pair<key_type,  mapped_type>(key, mapped_type()))));
		}
		key_compare key_comp() const {
			return(_comp);
		}

		value_compare value_comp() const {
			return (this->tree._comp);
		};

		friend bool operator==(const map &lhs, const map &rhs) {
			return ((lhs.size() == rhs.size()) && ft::equal<const_iterator, const_iterator>(lhs.begin(), lhs.end(), rhs.begin(),	rhs.end()));
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
//
//#ifdef DEBUG
//	public:
//		bool print() {
//			char tmp[1000] = {};
//			if (this->tree)
//				print_node(this->tree, 0, tmp);
//			return rb_test();
//		}
//		bool	rb_test()
//		{
//			if (is_rb_shaped(this->tree) == -1)
//			{
//				std::cout << "!!!!!!! NOT IN RB-SHAPE !!!!!!!!" << std::endl;
//				return (0);
//			}
//			return (1);
//		}
//	private:
//		int is_rb_shaped(node *n)
//		{
//			if (!n)
//				return (0);
//			if (exist(n->p) && n->p->color == RED && n->color == RED)
//				return (-1);
//			int r1 = is_rb_shaped(n->l);
//			int r2 = is_rb_shaped(n->r);
//			if (r1 == -1 || r2 == -1 || r1 != r2)
//				return (-1);
//			return (r1 + (n->color == BLACK));
//		}
//		void print_node(node* n, int depth, char *tmp)  {
//			if (n->l)
//				print_node(n->l, depth + 1, tmp);
//			for  (int i = 0; i < depth; i++)
//			{
//				if (i + 1 == depth)
//				{
//					if (n->p->l == n)
//					{
//						std::cout << "    ╔";
//						tmp[i] = 1;
//					}
//					else
//					{
//						std::cout << "    ╚";
//						tmp[i]  = 0;
//					}
//				}
//				else
//				{
//					if (tmp[i])
//						std::cout << "    ║";
//					else
//						std::cout << "     ";
//				}
//			}
//			if (n->color == RED)
//				std::cout << RED_OUTPUT;
//			if (n->color == BLACK)
//				std::cout << BLACK_OUTPUT;
//			std::cout << n->value.first << DEFAULT_OUTPUT;
//			if (n->value.first < 10 && (n->l || n->r))
//				std::cout << "═";
//			if (n->value.first < 100 && (n->l || n->r))
//				std::cout << "═";
//			if (n->value.first < 1000 && (n->l || n->r))
//				std::cout << "═";
//			if (n->r && n->l)
//			{
//				std::cout << "╣";
//				tmp[depth] = 1;
//			}
//			if (n->l && !n->r)
//			{
//				std::cout << "╝";
//				tmp[depth] = 0;
//			}
//			if (n->r && !n->l)
//			{
//				tmp[depth] = 1;
//				std::cout << "╗";
//			}
//			std::cout  << std::endl;
//
//			if (n->r)
//				print_node(n->r, depth + 1, tmp);
//		}
//#endif
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
