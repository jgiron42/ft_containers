//
// Created by joachim on 3/24/22.
//

#ifndef FT_CONTAINERS_RBTREE_HPP
#define FT_CONTAINERS_RBTREE_HPP
#include <stddef.h>
#include "../utils/swap.hpp"
#include <memory>
#include "../utils/pair.hpp"

#define _FT_MAP_PARENT_REF(n) (n->parent->right == n ? n->parent->right : n->parent->left)
#define _FT_MAP_SIB(n) (n->parent->right == n ? n->parent->left : n->parent->right)
#define _FT_MAP_CLO(n) (n->parent->right == n ? _FT_MAP_SIB(n)->right : _FT_MAP_SIB(n)->left)
#define _FT_MAP_DIS(n) (n->parent->right == n ? _FT_MAP_SIB(n)->left : _FT_MAP_SIB(n)->right)

template <typename T, class Compare, class Allocator = std::allocator<T> >
class RBtree
{
public:
	typedef T					value_type;
	typedef size_t				size_type;
	typedef Compare				value_compare;
	typedef Allocator				allocator_type;
	enum color {
		RED,
		BLACK
	};

	struct node
	{
		node	*right;
		node	*left;
		node	*parent;
		bool	color;
		T		value;
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
	private:
		iteratorT &increment(node *n, node *old) {
			if (n->left && old == n->parent)
				return (increment(n->left, n));
			if (old == n->parent)
			{
				this->pos = (node *)n;
				return (*this);
			}
			if (n->left && n->left == old)
			{
				this->pos = (node *)n;
				return (*this);
			}
			if (n->right && n->right != old)
				return (increment(n->right, n));
			if (n->parent)
				return (increment(n->parent, n));
			return (*this);
		}
		iteratorT &decrement(node * n, node * old) {
			if (n->right && old == n->parent)
				return (decrement(n->right, n));
			if (old == n->parent)
			{
				this->pos = (node *)n;
				return (*this);
			}
			if (n->right && n->right == old)
			{
				this->pos = (node *)n;
				return (*this);
			}
			if (n->left && n->left != old)
				return (decrement(n->left, n));
			if (n->parent)
				return (decrement(n->parent, n));
			return (*this);
		}
	};
	typedef iteratorT<value_type> iterator;
	typedef iteratorT<const value_type> const_iterator;
	node *tree;
	node superroot;
	node *first;
	node *last;
	size_type size;
	typename Allocator::template rebind<node>::other allocator;
	const Compare		&_comp;

	node	*new_node(node new_node)
	{
		node *ret = allocator.allocate(1);
		this->allocator.construct( ret, new_node);
		this->size++;
		return(ret);
	}
	void	delete_one_node(node *&n)
	{
		if (n == this->first)
			this->first = n->parent;
		if (n == this->last)
			this->last = n->parent;
		allocator.destroy(n);
		allocator.deallocate(n, 1);
		n = NULL;
		this->size--;
	}
	void	deep_delete_one_node(node *&n)
	{
		if (!n)
			return;
		if (n->right)
			deep_delete_one_node(n->right);
		if (n->left)
			deep_delete_one_node(n->left);
		delete_one_node(n);
	}
	node	*deep_copy_node(node n)
	{
		if (n.left)
			n.left = deep_copy_node(n->left);
		if (n.right)
			n.right = deep_copy_node(n->right);
		n.parent = NULL;
		node *ret = new_node(n->value, n);
		if (n.left)
			n.left->parent = ret;
		if (n.right)
			n.right->parent = ret;
	}
	node	*rotate_left(node* P) {
		node **ref = exist(P->parent) && P->parent->left == P ? &(P->parent->left) : (exist(P->parent) ? &(P->parent->right) : NULL);
		node *tmp = P->left;
		tmp->parent = P->parent;
		P->left = tmp->right;
		if (exist(tmp->right))
			tmp->right->parent = P;
		tmp->right = P;
		P->parent = tmp;
		if (!ref)
		{
			this->tree = tmp;
			this->tree->parent = &this->superroot;
			this->superroot.left = this->tree;
			this->superroot.right = this->tree;
		}
		else
			*ref = tmp;
		return (P);
	}
	node	*rotate_right(node* P) {
		node **ref = exist(P->parent) && P->parent->left == P ? &P->parent->left : (exist(P->parent) ? &P->parent->right : NULL);
		node *tmp = P->right;
		tmp->parent = P->parent;
		P->right = tmp->left;
		if (exist(tmp->left))
			tmp->left->parent = P;
		tmp->left = P;
		P->parent = tmp;
		if (!ref)
		{
			this->tree = tmp;
			this->tree->parent = &this->superroot;
			this->superroot.left = this->tree;
			this->superroot.right = this->tree;
		}
		else
			*ref = tmp;
		return (P);
	}
	bool	exist (node *n)
	{
		return (n && n != &this->superroot);
	}
	node *get_uncle(node *n) {
		if (!exist(n->parent))
			return (NULL);
		if (!exist(n->parent->parent))
			return (NULL);
		if (n->parent->parent->left == n->parent)
			return n->parent->parent->right;
		else
			return n->parent->parent->left;
	}
	void 	swap_node(node *a, node *b)
	{
		node *swap_node;

		if (a->right)
			a->right->parent = b;
		if (a->left)
			a->left->parent = b;
		if (b->right)
			b->right->parent = a;
		if (b->left)
			b->left->parent = a;
		swap_node = a->right;
		a->right = b->right;
		b->right = swap_node;

		swap_node = a->left;
		a->left = b->left;
		b->left = swap_node;

		if (a->parent->right == a)
			a->parent->right = b;
		if (a->parent->left == a)
			a->parent->left = b;
		if (b->parent->right == b)
			b->parent->right = a;
		if (b->parent->left == b)
			b->parent->left = a;
		swap_node = a->parent;
		a->parent = b->parent;
		b->parent = swap_node;
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
	void transplant(node *a, node *b)
	{
		if (a == a->parent->right)
			a->parent->right = b;
		if (a == a->parent->left)
			a->parent->left = b;
		if (b)
			b->parent = a->parent;
		if (a == this->tree)
			this->tree = b;
	}
	iterator recursive_lower_bound(node *n, const value_type &k, node *current) const
	{
		if (!n)
			return(iterator(current));
		if ((current == &superroot || _comp(n->value, current->value)) && !_comp(n->value, k))
		if ((current == &superroot || _comp(n->value, current->value)) && !_comp(n->value, k))
			current = n;
		if (_comp(k, n->value))
			return(recursive_lower_bound(n->left, k, current));
		if (_comp(n->value, k))
			return(recursive_lower_bound(n->right, k, current));
		return (iterator(current));
	}

	node	*deep_copy_tree(RBtree const &other, node *n)

	{
		if (!n)
			return (NULL);
		node new_node = {
				deep_copy_tree(other, n->right),
				deep_copy_tree(other, n->left),
				NULL,
				n->color,
				n->value
		};
		node * ret = this->new_node(new_node);
		if (n == other.first)
			this->first = ret;
		if (n == other.last)
			this->last = ret;
		if (new_node.left)
			new_node.left->parent = ret;
		if (new_node.right)
			new_node.right->parent = ret;
		return (ret);
	}
	void	reset_superroot()
	{
		this->superroot = (node){
			.right = this->tree,
			.left = this->tree,
		};
		if (this->tree)
			this->tree->parent = &this->superroot;
		if (this->first && this->first->left)
			this->first = this->first->left;
		if (this->last && this->last->right)
			this->last = this->last->right;
	}

	RBtree(const value_compare& comp = value_compare(),
		   const allocator_type& alloc = allocator_type()) : tree(NULL), superroot((node){}), first(&superroot), last(&superroot), size(0), allocator(alloc), _comp(comp) {}
	RBtree(RBtree const &other) : superroot((node){}), first(&superroot), last(&superroot), size(0), allocator(other.allocator), _comp(other._comp){
		tree = deep_copy_tree(other, other.tree);
		if (tree)
			tree->parent = &this->superroot;
		this->reset_superroot();
	}
	// RBtree(range)
	~RBtree()
	{
		this->clear();
	}
	RBtree &operator=(RBtree const &other)
	{
		this->clear();
		this->tree = deep_copy_tree(other, other.tree);
		this->reset_superroot();
		return (*this);
	}

	ft::pair<iterator, bool>	fix_insert(node *pos) {

		node *uncle;
		node *tmp = pos;
		start_of_balancing:
		if (tmp->parent->color == BLACK)
			return (ft::make_pair<iterator, bool>(iterator(pos), true));
		if (!exist(tmp->parent->parent)) // parent is root
		{
			tmp->parent->color = BLACK;
			return (ft::make_pair<iterator, bool>(iterator(pos), true));
		}
		uncle = get_uncle(tmp);
		if (exist(uncle) && uncle->color == RED) {
			uncle = get_uncle(tmp);
			tmp->parent->color = BLACK;
			uncle->color = BLACK;
			tmp->parent->parent->color = RED;
			tmp = tmp->parent->parent;
			if (exist(tmp) && exist(tmp->parent))
				goto start_of_balancing;
			return(ft::make_pair<iterator, bool>(iterator(pos), true));
		}

		if (tmp == tmp->parent->left) {
			if (tmp->parent->parent->right == tmp->parent) {
				rotate_left(tmp->parent);
				tmp->color = BLACK;
				tmp->parent->color = RED;
				rotate_right(tmp->parent);
			}
			else
			{
				tmp->parent->color = BLACK;
				tmp->parent->parent->color = RED;
				rotate_left(tmp->parent->parent);
			}
		}
		else {
			if (tmp->parent->parent->left == tmp->parent) {
				rotate_right(tmp->parent);
				tmp->color = BLACK;
				tmp->parent->color = RED;
				rotate_left(tmp->parent);
			}
			else
			{
				tmp->parent->color = BLACK;
				tmp->parent->parent->color = RED;
				rotate_right(tmp->parent->parent);
			}
		}
		return(ft::make_pair<iterator, bool>(iterator(pos), true));
	}
	ft::pair<iterator, bool>	recursive_insert(node *&pos, node *parent,  const value_type& value ) {
		if (!pos)
		{
			pos = new_node((node){NULL, NULL, parent, RED, value});
			this->reset_superroot();
			return (fix_insert(pos));
		}
		else if (_comp(value, pos->value))
			return (recursive_insert(pos->left, pos, value));
		else if (_comp(pos->value, value))
			return (recursive_insert(pos->right, pos, value));
		else
			return (ft::pair<iterator, bool>(iterator(pos), false));
	}

	iterator begin() {return (iterator(this->first));}
	const_iterator begin() const {return (const_iterator(this->first));}
	iterator end() {return (iterator(&this->superroot));}
	const_iterator end() const{return (const_iterator(&this->superroot));}
		// Modifiers
	ft::pair<iterator, bool>	insert( const value_type& value ) {
		return (recursive_insert(this->tree, &this->superroot, value));
	}
	ft::pair<iterator, bool>	insert(iterator hint,  const value_type& value ) {
		iterator tmp = hint;
		++tmp;
		if (hint.pos != &superroot && _comp(hint.pos->value, value) && (tmp.pos == &this->superroot || _comp(value, tmp.pos->value)))
		{
			if (!hint.pos->right)
				return (recursive_insert(hint.pos->right, hint.pos, value));
			else
				return (recursive_insert(tmp.pos->left, tmp.pos, value));
		}
		return (recursive_insert(tree, &this->superroot, value)); //fallback to classic insert
	}
	void erase( iterator pos )
	{
		node *n = pos.pos;
		if (this->first == n)
			this->first = (++iterator(n)).pos;
		if (this->last == n)
			this->last = (--iterator(n)).pos;
		if (pos.pos == this->tree && !n->right && !n->left)
		{
			delete_one_node(this->tree);
			this->tree = NULL;
			this->superroot.left = NULL;
			this->superroot.right = NULL;
			return;
		}
		if (n->left && n->right)
			swap_node(n, (++iterator(pos)).pos);
		if (n->color == RED)
		{
			_FT_MAP_PARENT_REF(n) = NULL;
			delete_one_node(pos.pos);
			return;
		}
		if (n->left && n->left->color == RED)
		{
			n->left->color = BLACK;
			this->transplant(n, n->left);
			n->left = NULL;
			delete_one_node(pos.pos);
			return;
		}
		if (n->right && n->right->color == RED)
		{
			n->right->color = BLACK;
			this->transplant(n, n->right);
			n->right = NULL;
			delete_one_node(pos.pos);
			return;
		}
		node *s;
		while (n != this->tree && n->color == BLACK)
		{
//				this->print();
			if (n == n->parent->left) {
				s = n->parent->right;
				if (s->color == RED) {
					s->color = BLACK;
					n->parent->color = RED;
					rotate_right(n->parent);
					s = n->parent->right;
				}
				if ((!s->left || s->left->color == BLACK) && (!s->right || s->right->color == BLACK)) {
					s->color = RED;
					n = n->parent;
				} else {
					if (!s->right || s->right->color == BLACK) {
						s->left->color = BLACK;
						s->color = RED;
						rotate_left(s);
						s = n->parent->right;
					}
					s->color = n->parent->color;
					n->parent->color = BLACK;
					if (s->right)
						s->right->color = BLACK;
					rotate_right(n->parent);
					n = this->tree;
				}
			} else {
				s = n->parent->left;
				if (s->color == RED) {
					s->color = BLACK;
					n->parent->color = RED;
					rotate_left(n->parent);
//						this->print();
					s = n->parent->left;
				}
				if ((!s->left || s->left->color == BLACK) && (!s->right || s->right->color == BLACK)) {
					s->color = RED;
					n = n->parent;
				} else {
					if (!s->left || s->left->color == BLACK) {
						s->right->color = BLACK;
						s->color = RED;
						rotate_right(s);
						s = n->parent->left;
					}
					s->color = n->parent->color;
					n->parent->color = BLACK;
					if (s->left)
						s->left->color = BLACK;
					rotate_left(n->parent);
					n = this->tree;
				}
			}
		}
		n->color = BLACK;
		_FT_MAP_PARENT_REF(pos.pos) = NULL;
		delete_one_node(pos.pos);
		return;
	}

	void	clear()
	{
		this->deep_delete_one_node(this->tree);
		this->reset_superroot();
	}
	void	swap(RBtree &other)
	{
		ft::swap(tree, other.tree);
		ft::swap(first, other.first);
		ft::swap(last, other.last);
		ft::swap(size, other.size);
//		ft::swap(_comp, other._comp);
		if (first == &other.superroot)
			first = &this->superroot;
		if (last == &other.superroot)
			last = &this->superroot;
		if (other.first == &superroot)
			other.first = &other.superroot;
		if (other.last == &superroot)
			other.last = &other.superroot;
		other.reset_superroot();
		this->reset_superroot();
	}

	// Lookup
	iterator find(const value_type &k) const
	{
		return (this->find(this->tree, k));
	}
	iterator find(node *n, const value_type &k) const
	{
		while (1) {
			if (!n)
				return(iterator(&this->superroot));
			else if (_comp(k, n->value))
				n = n->left;
			else if (_comp(n->value, k))
				n = n->right;
			else
				return (iterator(n));
		}
	}
	iterator upper_bound( const value_type& key ) const {
		iterator ret(recursive_lower_bound(this->tree, key, (node *)&this->superroot));

		if (!this->_comp(*ret, key) && !this->_comp(key, *ret) && ret != this->end())
			return (++ret);
		return (ret);
	}
	iterator lower_bound( const value_type& key ) const {
		return (recursive_lower_bound(this->tree, key, (node *)&this->superroot));
	}
};

#endif //FT_CONTAINERS_RBTREE_HPP
