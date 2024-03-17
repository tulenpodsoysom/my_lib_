#include "../geometry/line.hpp"
#include <iostream>

int main()
{
    double x1{},y1{};
    double x2{},y2{};
    while (true)
    {
        char key{};
        std::cin >> key;

        switch (key) {
        case 'w': y1 += 0.01;
        break;
        case 'a': x1 -= 0.01;
        break;
        case 's': y1 -= 0.01;
        break;
        case 'd': x1 += 0.01;
        break;

        case 'i': y2 += 0.01;
        break;
        case 'j': x2 -= 0.01;
        break;
        case 'k': y2 -= 0.01;
        break;
        case 'l': x2 += 0.01;
        break;
        case 'q': return 0;
		}
		line l(x1, y1, x2, y2,true);

        for (auto i = 1.0; i > 0.0; i -= 0.05) {
		    for (auto j = 0.0; j < 1.0; j += 0.05) {
                std::cout << ((abs(l(j,i)) > 0.02)? ' ' : '*');
		    } std::cout << std::endl;
	    }

        std::printf("x1:{%f},y1:{%f}\nx2:{%f},y2:{%f}\n",x1,y1,x2,y2);
        std::printf("A:{%f},B:{%f},C:{%f}\n\n",l.A,l.B,l.C);
    }
}
