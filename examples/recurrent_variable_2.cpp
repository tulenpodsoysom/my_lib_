#include "../recurrent_variable.hpp"
#include <iostream>
#include <cmath>

int main()
{
    recurrent_variable<double> rec;
    rec[0] = 0.1;
    
    rec.f = [] (auto& a) {return sin(a[0]);};

    std::cout << rec[0] <<std::endl;
    for (size_t i = 0; i < 20000; i++)
    {
        std::cout << rec() << std::endl;
    }
    
}