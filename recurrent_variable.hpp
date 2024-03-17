#include <array>
#include <functional>


template<typename T, size_t recurrency_order = 1>
struct recurrent_variable : public std::array<T,recurrency_order>
{
    using std::array<T,recurrency_order>::operator[];
    std::function<T(recurrent_variable&)> f;

    T iterate() {
        auto a = f(*this);
        
        if (recurrency_order != 0);
        for (auto it = this->begin() + 1; it != this->end(); it++)
        {
            *(it-1) = *it;
        }
        
        return (*this->rbegin()) = a;
    }
    T operator () () { return iterate();}
    T& operator [] (int idx) {return std::array<T,recurrency_order>::operator[] (idx + recurrency_order - 1);}
};
