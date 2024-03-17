#pragma once
#include <functional>

namespace my_geometry
{

    template<typename T = double>
    struct point2
    {
        T x{},y{};
    };
    template<typename T = double>
    struct point3
    {
        T x{},y{},z{};
    };
    typedef point3<double> point3d;
    typedef point3<float> point3f;
    typedef point3<int> point3i;
    typedef point3<unsigned> point3u;


    template<typename T>
    struct rect
    {
        T left,bottom,width,height;

        void create_size_dependent(T l, T b, T w, T h)
        {
            this->left = l;this->bottom = b;this->width = w;this->height = h;
        };

        void create_pos_dependent(T l, T t, T r, T b)
        {
            this->left = l;this->bottom = b;this->width = r - l;this->height = t - b;
        }

        void adjust(T dl, T dt, T dr, T db)
        {
            left += dl; bottom += db; width += dr - dl; height += dt - db;
        }

        bool includes(T x, T y)
        { 
            if ((x >= left) && (x <= left + width))
                if ((y >= bottom) && (y <= bottom + height)) return true;
            return false;
        }

        //T left() {return left;}
        //T bottom() {return bottom;}
        T right() {return left + width;}
        T top() {return bottom + height;}

        rect() = default;
        rect(T left,T bottom,T width, T height) : left(left),bottom(bottom),width(width),height(height) {}
    };

    template<typename T,typename Func>
    void iterate_over_left(rect<T>& r, size_t m, Func function)
    {
        for (int i = 0; i < m; ++i) {
            T x = r.left;
            T y = r.bottom + i *r.height/T(m);
            function(x,y);
        }
    }
    template<typename T,typename Func>
    void iterate_over_top(rect<T>& r, size_t n, Func function)
    {
        for (int i = 0; i < n; ++i) {
            T x = r.left + i*r.width/T(n);
            T y = r.top();
            function(x,y);
        }
    }
    template<typename T,typename Func>
    void iterate_over_right( rect<T>& r, size_t m, Func function)
    {
        for (int i = 0; i < m; ++i) {
            T x = r.right();
            T y = r.top() - i*r.height/T(m);
            function(x,y);
        }
    }
    template<typename T,typename Func>
    void iterate_over_bottom( rect<T>& r, size_t n, Func function)
    {
        for (int i = 0; i < n; ++i) {
            T x = r.right() - i*r.width/T(n);
            T y = r.bottom;
            function(x,y);
        }
    }


    //from top-left corner, clock-wise
    template<typename T,typename Func>
    void iterate_over_rect(rect<T> r, size_t n, size_t m, Func function)
    {
        iterate_over_top(r,n,function);
        iterate_over_right(r,m,function);
        iterate_over_bottom(r,n,function);
        iterate_over_left(r,m,function);
    }


    

    template<typename T = double>
    struct line
    {
        T x1{},y1{},x2{},y2{};

        line() = default;
        line(T x1,T y1,T x2,T y2) :  x1{x1},y1{y1},x2{x2},y2{y2} {};

        auto lerp(double t) {return {x1 + (x2-x1)*t, y1 + (y2-y1)*t};}
    };

    template<typename T = double>
    struct plane
    {
        // A*x + B*y + C*z + D = 0
        T A{},B{},C{},D{};

        void create_from_points(point3<T> p1, point3<T> p2,point3<T> p3)
        {
            auto v1 = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
            auto v2 = {p3.x - p1.x, p3.y - p1.y, p3.z - p1.z};

            A =   v1[1]*v2[2] -  v1[2]*v2[1];
            B = -(v1[0]*v2[2] -  v1[2]*v2[0]);
            C =   v1[0]*v2[1] -  v1[1]*v2[0];
            D = - A * p1.x - B * p1.y - C * p1.z;
        }

        void create_from_points(
            double x1, double y1, double z1, 
            double x2, double y2, double z2, 
            double x3, double y3, double z3 
        )   {create_from_points({x1,y1,z1},{x2,y2,z2},{x3,y3,z3});}

        void create_from_normal(point3<T> normal, point3<T> origin)
        {
            A = normal.x;
            B = normal.y;
            C = normal.z;
            D = -A * origin.x - B * origin.y - C * origin.z;
        }

        T norm() {return std::sqrt(A*A+B*B+C*C);};
        T norm2() {return A*A+B*B+C*C;};
        T distance_to(point3<T> p) {return (A*p.x + B*p.y + C*p.z + D)/ norm(); }
    };
    
    
} // namespace my_geometry
