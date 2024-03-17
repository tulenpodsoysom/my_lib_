#include <vector>
#include <iostream>
#include "../linspace.hpp"

int main()
{
    discrete_linspace x,y;
    x.create_bound_dependent(1,-1,5);
    y.create_bound_dependent(1,-1,5);


    std::cout << "bound_dependent variant:" << std::endl;

    std::cout << "x.left_bound: " << x.left_bound() << "\tx.right_bound: " << x.right_bound() << std::endl;
    std::cout << "y.left_bound: " << y.left_bound() << "\ty.right_bound: " << y.right_bound() << std::endl;

    std::cout << "x.size(): " << x.size() << std::endl;
    std::cout << "y.size(): " << y.size() << std::endl;

    for (auto &j : x)
    {
        for (auto &i : y)
        {
            std::cout << "X: " << j << " Y: " << i << std::endl;
        }
    }
    
    x.create_width_dependent(1,-2,5);
    y.create_width_dependent(1,-2,5);

    std::cout << "width_dependent variant:" << std::endl;

    std::cout << "x.left_bound: " << x.left_bound() << "\tx.right_bound: " << x.right_bound() << std::endl;
    std::cout << "y.left_bound: " << y.left_bound() << "\ty.right_bound: " << y.right_bound() << std::endl;

    for (auto &j : x)
    {
        for (auto &i : y)
        {
            std::cout << "X: " << j << " Y: " << i << std::endl;
        }
    }
    

}