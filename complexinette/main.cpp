#include <set>
#include <map>
#include <iomanip>
#include <iostream>
#include <vector>
#include "../srcs/containers/map.hpp"
#include "../srcs/utils/pair.hpp"
#ifndef NTEST
#define NTEST 10000
#endif
#ifndef MAX_SIZE
#define MAX_SIZE 6
#endif
#include <cmath>
template <class C, typename ft, class ... Types>
float	time_function(C &init, ft f, Types ... args)
{
	float	ret = 0;
	clock_t	begin;
	C		tmp;

	for (int i = 0; i < NTEST; ++i)
	{
		tmp = init;
		begin = clock();
		(tmp.*f)(args...);
		ret += (float)(clock() - begin);
	}
	return (ret / NTEST);
}

void render(std::map<float, int> &render)
{
	float yscale = (--render.end())->first;
	float min_diff = yscale;
	float tmp = -1;
	for (auto i:render) {
		std::cout << i.first << " -> " << i.second << std::endl;
		if (tmp != -1) {
			if (i.first - tmp < min_diff)
				min_diff = i.first - tmp;
		}
		tmp = i.first;
	}
	std::cout << "yscale: " << yscale << " min_diff: " << min_diff << " i init: " << int(yscale / min_diff) + 1 << std::endl;
	auto it = render.end();
	--it;
	for (int i = int(yscale / min_diff) + 1; i > 0; i--)
	{
		if (it->first <= i * min_diff && it->first > (i - 1) * min_diff)
		{
			std::cout << std::setfill(' ') << std::setw(it->second) << "o";
			--it;
		}
		std::cout << std::endl;
	}

}

float comp_const(int n) {return (1);}
float comp_primconst(float n) {return (n);}

float comp_log(int n) {return (log(n));}
float comp_primlog(float n) {return (n * log(n) - n);}

float comp_sqrt(int n) {return (sqrt(n));}
float comp_primsqrt(float n) {return ((2/3) *powl(sqrt(n), 3));}

float comp_linear(int n) {return (n);}
float comp_primlinear(float n) {return (n * n / 2);}

float comp_linearithmique(int n) {return (n * log(n));}
float comp_primlinearithmique(float n) {return (- (n * n) / 4 + (n * n) * log(n) / 2);}

float comp_quad(int n) {return (n * n);}
float comp_primquad(float n) {return ((n * n * n) / 3);}

float comp_cub(int n) {return (n * n * n);}
float comp_primcub(float n) {return ((n * n * n * n) / 4);}

float comp_exp(int n) {return ((int)1 << n);}
float comp_primexp(float n) {return (((int)1 << (int)n) / log(2));}

float comp_fact(int n) {
	if (n == 0)
		return (1);
	return (n * comp_fact(n - 1));
}


float comp_primfact(float n) {
	float ret = 0;
	while ( n > 0)
	{
		ret += comp_fact((int)n);
		--n;
	}
	return (ret);
}

float evaluate_comp(std::map<int, float> &result, float (*f)(int))
{
	float min = MAXFLOAT;
	float max = 0;
	float average = 0;
	float a = 0;
	float b = 0;
	float tmp;
	for (auto i: result)
		average += f(i.first) / i.second;
	average /= result.size();
	for (auto i: result)
	{
		tmp = (i.second - (f(i.first) + f(result.begin()->first) - result.begin()->second));
		a += tmp * tmp;
		b += i.second * i.second;
//		std::cout << "first: " << i.first << " second: " << i.second << " tmp: " << tmp << std::endl;
//		if ( tmp > max )
//			max = tmp;
//		if ( tmp < min )
//			min = tmp;
	}
	return (sqrt(a / b));
}

void aprox(std::map<int, float> &result)
{
	float (*complexities[])(int) = {
			&comp_const,
			&comp_log,
			&comp_sqrt,
			&comp_linear,
			&comp_linearithmique,
			&comp_quad,
			&comp_cub,
			&comp_exp,
			&comp_fact,
			NULL
	};
	float (*primcomplexities[])(float) = {
			&comp_primconst,
			&comp_primlog,
			&comp_primsqrt,
			&comp_primlinear,
			&comp_primlinearithmique,
			&comp_primquad,
			&comp_primcub,
			&comp_primexp,
			&comp_primfact,
			NULL
	};
	for (int i = 0; complexities[i]; i++)
		std::cout <<  evaluate_comp(result, complexities[i]) << std::endl;
}

#ifndef NAMESPACE
# define NAMESPACE ft
#endif

typedef NAMESPACE::map<int, int> C;
typedef NAMESPACE::pair<C::iterator, bool> (C:: *inserttype)(const C::value_type &);
int main(int, char **)
{
	std::vector<C>		m;
	std::vector<NAMESPACE::pair<int, int> >	v;
	std::map<float, int> renderm;
	std::map<int, float> result;
	for (int i = 0, j = 0, k = 1; i < powl(10, MAX_SIZE); i++)
	{
		v.insert(v.end(),NAMESPACE::pair<int, int>(i, i));
		if (i == k)
		{
			m.push_back(C(v.begin(), v.end()));
			j++;
			k *= 10;
		}
	}
	float ret;
	for (int i = 0, j = 1; i < m.size(); i++, j *= 10)
	{
		ret = time_function<C, inserttype , const C::value_type & >(m[i], &C::insert, C::value_type(j + 1, j + 1));
		std::cout << j << "," << ret << std::endl;
		renderm[ret] = j;
		result[j] = ret;
	}
//	aprox(result);
//render(renderm);
}