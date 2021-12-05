#include "srcs/containers/vector.hpp"
#include <vector>

using namespace std;

int main()
{
	vector<int> v;
	
	v.insert<int>(v.begin(), 12, 13);
}
