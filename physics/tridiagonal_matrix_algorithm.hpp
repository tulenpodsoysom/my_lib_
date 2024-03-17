#pragma once
#include <vector>
#include <functional>

//      evaluated for a matrix 
//      A(i) x[i-1] * B(i) x[i] * C(i) x[i+1] = D(i)

// Func must be dependent on an index;
template<typename T, typename Func = std::function<T(unsigned)>>
struct run_through_method
{
    Func A;
    Func B;
    Func C;
    Func D;

    std::vector<T> alpha;
    std::vector<T> beta;

    std::vector<T> output;

    // a1 and b1 are dependent on a boundary condition on the left (from right to the left)
    // a2 and b2 are dependent on a boundary condition on the right (from left to the right)
    // n is a size of X array
    // don't forget to define A, B, C, D.
    void evaluate(size_t n, T a1, T b1,T a2,T b2)
    {
        output.resize(n,{});
         alpha.resize(n+1,{});
          beta.resize(n+1,{});

        alpha[0] = 0;
        beta [0] = 0;
        alpha[1] = a1;
        beta [1] = b1;
        //alpha[n-1] = a2;
        //beta [n-1] = b2;

        for (size_t i = 1; i < n; i++)
        {
            // n-1 is last elem
            alpha[i + 1] = -C(i) / (A(i) * alpha[i] + B(i));
            beta[i + 1] = (D(i) - A(i) * beta[i]) / (A(i) * alpha[i] + B(i));
        }

        //output[n-1] = Xright;
        output[n-1] = (a2 * beta[n] + b2)/(1.0-a2*alpha[n]);
        for (int i = n - 2; i >= 0; i--)
        {
            output[i] = alpha[i+1] * output[i+1] + beta[i+1];
        }
    };
};
