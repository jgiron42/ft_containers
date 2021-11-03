//
// Created by Joachim GIRON on 7/18/21.
//

#ifndef FT_CONTAINERS_STACK_HPP
#define FT_CONTAINERS_STACK_HPP
#include <vector>
namespace ft {
	template<	class T, class Container = std::vector<T> >
	class stack{
	public:
		typedef Container 					container_type;
		typedef typename Container::value_type		value_type;
		typedef typename Container::size_type		size_type;
		typedef typename Container::reference		reference;
		typedef typename Container::const_reference	const_reference;
	protected:
		container_type c;
	public:
		explicit stack(const Container& cont = Container()) : c(cont) {};
		stack( const stack& other ) : c(other.c) {};
		~stack() {};
		stack& operator=( const stack& other ) {this->c = other.c;};
		reference top() {return (c.back());};
		const_reference top() const {return (c.back());};
		bool empty() const {return (c.empty());};
		size_type size() const { return (c.size());};
		void push( const value_type& value ) {c.push_back(value);};
		void pop() {c.pop_back();};


		friend bool operator==( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ){
			return(lhs.c == rhs.c);
		};
		friend bool operator!=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ){
			return(lhs.c != rhs.c);
		};
		friend bool operator<( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ){
			return(lhs.c < rhs.c);
		};
		friend bool operator<=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ){
			return(lhs.c <= rhs.c);
		};
		friend bool operator>( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ){
			return(lhs.c > rhs.c);
		};
		friend bool operator>=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ){
			return(lhs.c >= rhs.c);
		};

	};

}
#endif //FT_CONTAINERS_STACK_HPP