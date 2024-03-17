#include "../physics/piecewise_function.hpp"

#include <iostream>
#include <cmath>
#include <utility>

int main()
{
    piecewise_function<double,double> f {
            {-INFINITY  ,[](double x) {return cos(x);}},
            {0          ,[](double x) {return sin(x);}}
        };

    for (double x = -2.0; x < 2.0; x += 0.1)
    {
        std::cout << "x: " << x << "\tf(x): " << f(x) << std::endl;
    }
    
}