#ifndef FFT_H
#define FFT_H
#include <vector>
#include <complex>
#include <algorithm>

template <typename InputIt, typename OutputIt>
void abs_transform(InputIt begin, InputIt end, OutputIt out)
{
    std::transform(begin,end,out,[](auto& a){return abs(a);});
}


template<typename InputIt, typename OutputIt>
void fourier(InputIt begin, InputIt end, OutputIt out, double is)
{
    int size = end - begin;
    if (size & size - 1) throw "size is not a power of two";
    static const double PI = 3.14159265358979323;

    //if constexpr (
    //    InputIt::value_type != std::complex<double> && 
    //    InputIt::value_type != std::complex<float>) throw "container argument must be complex";

    if (begin != out) {std::copy(begin,end,out);}

    std::complex<double> temp, w, c;
    long i, j, istep;
    long m, mmax;
    long n = end - begin;
    double fn, r1, theta;
    fn=(double)n;
    double r = PI * is;

    j = 1;
    for( i=1; i<=n; i++)
    {
        long i1 = i - 1;
        if(i<j)
        {
            int j1=j-1;
            std::swap(*(out + j1),*(out + i1));

        }
        m=n/2;
        while( j > m ){	j -= m;	m = (m+1)/2; }
        j+=m;
    }
    mmax=1;
    while(mmax<n)
    {
        istep=2*mmax;
        r1=r/(double)mmax;
        for( m=1; m<=mmax; m++)
        {
            theta = r1*(double)(m-1);
            w = std::polar(1.0,theta);
            for( i=m-1; i<n; i+=istep)
            {
                j=i+mmax;
                c = *(out + j);
                temp = w * c;
                *(out + j) = *(out + i) - temp;
                *(out + i) = *(out + i) + temp;
            }
        }
        mmax=istep;
    }
    if (is>0) for (i=0; i<n; i++)
    {
        *(out + i) /= fn;
    }
};


template<typename InputIt>
std::complex<double> frequency_fourier(InputIt begin, InputIt end, double frequency, double is) {
    
    static const double PI = 3.14159265358979323;
    std::complex<double> result(0.0, 0.0);

    int N = end - begin;// data.size();

    for (int n = 0; n < N; ++n) {
        result += *(begin+n) * std::polar(1.0, is*2.0 * PI * frequency * n / N);
    }

    return result;
}


/*

std::vector<std::complex<double>> fourier(std::vector<std::complex<double>> F, double is)
{
    std::complex<double> temp, w, c;
    long i, j, istep;
    long m, mmax;
    long n = F.size();
    double fn, r1, theta;
    fn=(double)n;
    double r = PI * is;

    j = 1;
    for( i=1; i<=n; i++)
    {
        long i1 = i - 1;
        if(i<j)
        {
            int j1=j-1;
            temp = F[j1];
            F[j1] = F[i1];
            F[i1] = temp;
        }
        m=n/2;
        while( j > m ){	j -= m;	m = (m+1)/2; }
        j+=m;
    }
    mmax=1;
    while(mmax<n)
    {
        istep=2*mmax;
        r1=r/(double)mmax;
        for( m=1; m<=mmax; m++)
        {
            theta = r1*(double)(m-1);
            w = std::polar(1.0,theta);
            //w = std::complex<double>(cos(theta), sin(theta));
            for( i=m-1; i<n; i+=istep)
            {
                j=i+mmax;
                c = F[j];
                temp = w * c;
                F[j] = F[i] - temp;
                F[i] = F[i] + temp;
            }
        }
        mmax=istep;
    }
    if (is>0) for (i=0; i<n; i++)
    {
        F[i] /= fn;
    }
    return F;
};
*/

#endif // FFT_H
