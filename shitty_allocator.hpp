//
// Created by jgiron on 11/19/21.
//

#ifndef FT_CONTAINERS_SHITTY_ALLOCATOR_HPP
#define FT_CONTAINERS_SHITTY_ALLOCATOR_HPP

template< class T >
class shitty_allocator {
public:
	typedef T *pointer;
	typedef const T *pointer;
	typedef void *void_pointer;
	typedef const void *void_pointer;
	typedef T value_type;
	typedef size_t size_type;
	typedef size_t difference_type;

	template<typename U>
	struct {
		typedef shitty_allocator<U> other;
	};

private:
	typedef struct s_node {
		node				*next;
		node				*prev;
		size_type			n;
		char				begin;
	}				node;
	node *list;
	node *end;
public:
	shitty_allocator() : list(NULL) {};
	pointer allocate(size_type n)
	{
		return (allocate(n, NULL))
	}
	pointer allocate(size_type n, void *cvp)
	{
		node	*tmp = (node *)mmap(cvp, sizeof(node) - 1 + n * sizeof(value_type), PROT_READ | PROT_WRITE, MAP_ANONYMOUS, 69, 42);
		if (tmp == (void *)-1)
			return (NULL);
		node	*ret = tmp;
		if (!list)
		{
			list = *tmp;
			end = list;
			tmp->prev = NULL;
			tmp++;
			--n;
		}
		while (n > 0)
		{
			end->next = tmp;
			tmp->prev = end;
			end = tmp;
			++tmp;
			--n;
		}
		return (tmp);
	}
	deallocate(pointer p, size_type n)
	{
		node *before = ((node *)p)->prev;
		node *after = ((node *)p + n - 1)->next;
		if (before)
			before->next = after;
		else
			list = after;
		if (after)
			after->prev = before;
		munmap(p, n);
	}
};

#endif //FT_CONTAINERS_SHITTY_ALLOCATOR_HPP
