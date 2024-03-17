#pragma once
#include <vector>
#include <cmath>

class discrete_linspace : public std::vector<double>
{
    double l_bound, r_bound;
public: 
    using std::vector<double>::operator[];

    discrete_linspace() {}
    void create_bound_dependent(double l, double r, size_t divisions, bool include_right_bound = false) 
    { 
        this->resize(divisions); 
        for (size_t i = 0; i < this->size(); i++)
        {
            double d = divisions - (include_right_bound? 1 : 0);
			(*this)[i] = l + (r - l) * i / d;
        }

        l_bound = l;
        r_bound = r;
    }
    void create_width_dependent(double l,double width, size_t divisions)
    {
        this->resize(divisions);
        for (size_t i = 0; i < this->size(); i++)
        {
            (*this)[i] = l + width * i / (double)divisions; 
        }
        l_bound = l;
        r_bound = l + width;
    }
    void create_step_dependent(double l, double step, size_t divisions)
    {
        this->resize(divisions);
        for (size_t i = 0; i < this->size(); i++)
        {
            (*this)[i] = l + i * step; 
        }
        l_bound = l;
        r_bound = l + step * divisions;
    }

    const double& left_bound() {return l_bound;}
    const double& right_bound() {return r_bound;}

    double closest_round(double x) {
        return left_bound() + round((x - left_bound())/get_step()) * get_step();
    }
    double closest_ceil(double x) {
        return left_bound() + ceil((x - left_bound())/get_step()) * get_step();
    }
    double closest_floor(double x) {
        return left_bound() + floor((x - left_bound())/get_step()) * get_step();
    }


    size_t closest_index(double x)
    {
        auto a = round((x - left_bound())/get_step());
        if (a > size()-1) return size()-1;
        if (a < 0) return 0;
        return a;
    }
    size_t closest_ceil_index(double x)
    {
        auto a = ceil((x - left_bound())/get_step());
        if (a > size()-1) return size()-1;
        if (a < 0) return 0;
        return a;
    }
    size_t closest_floor_index(double x)
    {
        auto a = floor((x - left_bound())/get_step());
        if (a > size()-1) return size()-1;
        if (a < 0) return 0;
        return a;
    }

    double get_step() {return (r_bound - l_bound)/ size();}
    double width() { return r_bound - l_bound; }
};

