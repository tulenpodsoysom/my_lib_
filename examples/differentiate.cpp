#include "../math_functions.hpp"
#include "../graphics/PlotImage.hpp"
#include <iostream>

using namespace std;

int main()
{
    my_graphics::PlotImage plot1;

    vector<double> signal1(1024);
    vector<double> signal2(1024);
    for (size_t i = 0; i < 1024; i++)
    {
        signal1[i] = sin(i/1024.);
    }

    plot1.set_points(signal1.begin(),signal1.end());

    my_functions::differentiate(1./1024., signal1.begin(),signal1.end(),signal2.begin());
    plot1.add_points(signal2.begin(),signal2.end());

    plot1.save_as_file(L"output.png",720,480);
}