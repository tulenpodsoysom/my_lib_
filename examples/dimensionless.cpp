#include <iostream>
#include "../physics/dimensionless.hpp"

struct my_var : public dimensionless_variables
{
    def_variable(x,x0,3.1415926535);
    def_variable(y,y0,2*3.1415926535);
};
// expands to
// double x0 = 3.1415926535;   double x_to_x0(double a) {return a/x0;}; double x0_to_x(double a) {return a * x0;}
// double y0 = 2*3.1415926535; double y_to_y0(double a) {return a/y0;}; double y0_to_y(double a) {return a * y0;}


int main()
{
    my_var v;
    for (int i = 0; i <= 5; i++)
    {
        auto a = 3.1415926535/5. * i;
        std::cout << i <<  ":\t x0: " << v.x_to_x0(a) << "\tx: " << v.x0_to_x(a) << "\ty0: " << v.y_to_y0(a)<< "\t y:" << v.y0_to_y(a) << std::endl;
    }
    return 0;
}