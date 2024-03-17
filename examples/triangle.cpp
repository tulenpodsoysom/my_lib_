#include "../geometry/triangle.hpp"
#include <iostream>

using namespace std;
int main() 
{
	geometry::triangle2<double> t{{0.,1.},{0.5,0.},{0.75,0.75}};

	for (auto i = 1.0; i > 0.0; i -= 0.05) {
		for (auto j = 0.0; j < 1.0; j += 0.05) {
            cout << (t.contains(j, i)? ' ' : '*');
		} cout << endl;
	}
}