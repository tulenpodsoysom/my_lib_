
#include <functional>
#include <map>

template <typename R, typename T>
struct piecewise_function
{
    piecewise_function(std::initializer_list<std::pair<double,std::function<R(T)>>> l)
    {
        for (auto &i : l)
        {
            functions[i.first] = i.second;
        }
    }

    // double defines a left_bound of a function;
    std::map<double,std::function<R(T)>> functions;
    
    R operator() (T x)
    {
        if (functions.empty()) return R{};
        if (x < functions.begin()->first) return R{};

        auto f = --functions.lower_bound(x);
        return f->second(x);
    }
};