
#include "../recurrent_variable.hpp"
#include <iostream>

int main()
{
    //
    //  Пример - последовательность фиббоначи
    //

    recurrent_variable<double,2> rec;

    rec[-1] = 0.0;
    rec[ 0] = 1.0;
    

    // Последовательность задается выражением Fn+1 = Fn + Fn-1
    rec.f = [] (auto& a) {return a[0] + a[-1];};

    std::cout << rec[-1] <<std::endl;
    std::cout << rec[ 0] <<std::endl;

    for (size_t i = 0; i < 20; i++)
    {
        std::cout << rec() << std::endl;
    }
    
}