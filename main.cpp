#include "map.hpp"
#include <string>
#include "pair.hpp"
#include "vector.hpp"
#include <map>
#include <vector>
#include <unistd.h>
#include "test/test_map.hpp"
#include "test/test_vector.hpp"
#ifndef NAMESPACE
# define NAMESPACE ft
# define ZBOUB "ft"
#endif

int main(int argc, char **argv)
{
	int seed = 0;

	if (argc >= 2)
	{
		if (argc == 3)
			seed = atoi(argv[2]);
		if (std::string(argv[1]) == std::string("vector"))
		{
			test_vector<NAMESPACE::vector<std::string> >(seed);
		}
		else if (std::string(argv[1]) == std::string("map"))
		{
			test_map<NAMESPACE::map<std::string, std::string>, NAMESPACE::pair>(seed);
		}
		else
		{
			std::cerr << "Invalid arguments" << std::endl;
			return (1);
		}
		if (!isatty(1))
		{
			std::cout << (char)'\3' << std::endl;
		}
	}
} 
