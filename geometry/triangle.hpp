#ifndef TRIANGLE_H
// #include "point.hpp"

namespace geometry {


    template <typename T>
    struct triangle2
    {
		//triangle2<T>(point2<T> p1, point2<T> p2, point2<T> p3)
		//	: p1(p1), p2(p2), p3(p3) {}
		//triangle2<T>(triangle2<T>&) = default;
        
        point2<T> p1,p2,p3;


        bool contains(T x, T y)
        {
            auto sign =
                [](point2<T> p1, point2<T> p2, point2<T> p3) {
                    return (p1.x() - p3.x()) * (p2.y() - p3.y()) -
                       (p2.x() - p3.x()) * (p1.y() - p3.y());
                };
            double d1, d2, d3;
            bool has_neg, has_pos;

            d1 = sign({x,y}, p1, p2);
            d2 = sign({x,y}, p2, p3);
            d3 = sign({x,y}, p3, p1);

            has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
            has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

            return !(has_neg && has_pos);
        }
        double perimeter()
        {
            double a = distance(p1, p2);
            double b = distance(p2, p3);
            double c = distance(p3, p1);
            return a + b + c;
        }
        double semiperimeter()
        {return perimeter() / 2.0;}

        double area()
        {
            double a = distance(p1, p2);
            double b = distance(p2, p3);
            double c = distance(p3, p1);

            //semiperimeter
            T sp = (a+b+c)/2.0;
            return sqrt(sp*(sp-a)*(sp-b)*(sp-c));
        }
    };


    template <typename T>
    triangle2<T> expand_triangle(triangle2<T> t, double scale)
    {
        auto t_ = t;
        auto p = middle_point({t_.p1,t_.p2,t_.p3});
        t_.p1 = lerp(t_.p1, p, scale);
        t_.p2 = lerp(t_.p2, p, scale);
        t_.p3 = lerp(t_.p3, p, scale);

        return t_;
    }

}

#endif // TRIANGLE_H
#define TRIANGLE_H
