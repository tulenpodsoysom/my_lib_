#include "../FFT.hpp"

#include <iostream>

template<class T>
void print(std::string prefix, const T& a)
{
    std::cout << prefix << std::endl;
    for (size_t i = 0; i < a.size(); i++)
    {
        std::cout << a[i] << std::endl;
    }
}


int main()
{
    unsigned N = 8;


    std::vector<std::complex<double>> vals(N);
    for (size_t i = 0; i < N; i++)
    {
        vals[i] = cos(2 * 3.14159265358979323 * i / (double) N);
    }
    print("\nf(t) cosine",vals);
    
    fourier(vals.begin(),vals.end(),vals.begin(),-1);
    print("\nF(f) cosine",vals);
    
    std::vector<double> absolute_spectre(N);
    abs_transform(vals.begin(),vals.end(),absolute_spectre.begin());
    print("\nAbsolute spectre of cosine",absolute_spectre);

    fourier(vals.begin(),vals.end(),vals.begin(),1);
    print("\nf(t) cosine from F(f)",vals);

}
