#pragma once

#include <cmath>
#include <vector>
#include <algorithm>
#include <complex>
#include <functional>

namespace my_functions
{
    static const double pi = 3.14159265358979323;
    static const double two_pi = 2*3.14159265358979323;
    
    struct  sine_params{double a = 1.0, f = 1.0, phase = 0.0;};
    struct gauss_params{double a = 1.0, sigma = 1.0, mean = 0.0;};

    inline double  sine_func(double t, sine_params p = {}) { return p.a * sin(two_pi * p.f * t + p.phase); };
    inline double gauss_func(double x, gauss_params p = {}, bool normalized = 0) {
        return p.a * exp(-(x - p.mean) * (x - p.mean) / (2 * p.sigma * p.sigma)) * (normalized ? 1.0 / (sqrt(two_pi) * p.sigma) : 1.0);
    };

    struct sine_generator
    {
        sine_params p;
        double fd;
        sine_generator(double fd, double a = 1.0, double f = 1.0, double ph = 0.0) : fd{ fd }, p{ a, f, ph } {}
        sine_generator(double fd, sine_params p = {}) : fd{ fd }, p{ p } {}

        int n = 0;
        double operator () () { return sine_func(n++ / fd, p); }
    };

    struct gauss_generator
    {
        gauss_params p;
        double fd;
        bool normalized = false;
        gauss_generator(double fd, double a = 1.0, double sigma = 1.0, double mean = 0.0, bool n = false) : fd{ fd }, p{ a, sigma, mean }, normalized{ n } {}
        gauss_generator(double fd, gauss_params p = {}, bool n = false) : fd{ fd }, p{ p }, normalized{ n } {}

        int n = 0;
        double operator () () { return gauss_func(n++ / fd, p, normalized); }
    };



    template <typename T>
    std::vector<typename T::value_type> correlation(T a_begin, T a_end, T b_begin, T b_end)
    {
        std::vector<typename T::value_type> correlations(a_end - a_begin);

        for (size_t i = 0; i < a_end - a_begin; i++)
        {
            for (size_t j = 0; j < b_end - b_begin; j++)
            {
                if constexpr (std::is_same<typename T::value_type, std::complex<double>>::value)
                {
                    correlations[i] += *(a_begin + j) * conj(*(b_begin + (j - i) % (b_end - b_begin)));
                }
                else correlations[i] += *(a_begin + j) * *(b_begin + (j - i) % (b_end - b_begin));
            }
        }

        return correlations;
    };




    template <typename T>
    std::vector<typename T::value_type> autocorrelation(T begin, T end)
    {
        return correlation<T>(begin, end, begin, end);
    };


    template<typename T = double>
    std::pair<T, T> divide_by_two_method(std::function<T(T)> f, T left, T right, T epsilon, T compare_to)
    {
    	T c;
    	do
    	{
    		c = (right + left) / 2.0;
    		if ((f(c) - compare_to) * (f(left) - compare_to) < 0)
    		{
    			right = c;
    		}
    		else
    		{
    			left = c;
    		}
    	} while (abs(right - left) >= epsilon);
    
    	return { f(c),c };
    };

    template<typename InputIt, typename OutputIt>
    void differentiate(double dx, InputIt begin, InputIt end, OutputIt output)
    {
        if (end - begin < 3) throw "too few values";
    
        *output = (-3.0 * (*begin) + 4.0 *(*(begin + 1)) - *(begin + 2))/2./dx;
        for (int var = 1; var < end - begin - 1; ++var) {
            *(output + var) = (*(begin+ var + 1) - *(begin + var - 1)) / 2.0/dx;
        }
        *(output + (end - begin) - 1) = (*(end - 3) -4.0 *(*(end -2)) + 3.0*(*(end - 1)))/2./dx;
    }

} // namespace my_functions
