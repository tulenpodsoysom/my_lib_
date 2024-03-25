#ifndef RECT_H
#include "point.hpp"

#include <type_traits>

namespace geometry
{
    template <typename T>
    struct rect
    {
        T left,bottom,width,height;

        rect() = default;
        rect(T left,T bottom,T width, T height) : left(left),bottom(bottom),width(width),height(height) {}

        inline constexpr void create_size_dependent(T l, T b, T w, T h)
        {
            this->left = l;this->bottom = b;this->width = w;this->height = h;
        };

        inline constexpr void create_pos_dependent(T l, T t, T r, T b)
        {
            this->left = l;this->bottom = b;this->width = r - l;this->height = t - b;
        }

        inline constexpr void adjust(T dl, T dt, T dr, T db)
        {
            left += dl; bottom += db; width += dr - dl; height += dt - db;
        }

        bool contains(T x, T y)
        {
            if (
                (x > left) && (x < left + width) && 
                (y < bottom + height) && (y > bottom) 
            ) return true;
            else return false;
        }

        bool on_edge(T x, T y)
        {
            if (std::is_floating_point_v<T> == true) throw "floating point equality check";
            
            if ((x == left) || (x == right())) { if ((y < top()) && (y > bottom)) return true;}
            if ((y == top()) || (y == bottom)) { if ((x > left) && (x < right())) return true;}

            return false;
        }


        inline constexpr T top(){return bottom + height;}
        inline constexpr T right(){return left + width;}

        inline constexpr T area()
        {
            return width * height;
        }

		inline constexpr point2<T> left_top() { return {left, top()}; };
		inline constexpr point2<T> left_bottom() { return {left, bottom}; };
		inline constexpr point2<T> right_top() { return {right(), top()}; };
		inline constexpr point2<T> right_bottom() { return {right(), bottom}; };

		inline constexpr point2<T> from_rect_coord(double mu_x, double nu_y)
        {
			return point2<T>(left + width * mu_x, bottom + height * nu_y);
		}

        point2<T> lerp_left_side(double mu) {
            if (mu < 0.0)
                return {left, bottom};
            else if (mu > 1.0)
                return {left, top()};
            else
             return {left,bottom + mu*height};
        }
        point2<T> lerp_right_side(double mu) {
            if (mu < 0.0)
                return {right(), bottom};
            else if (mu > 1.0)
                return {right(), top()};
            else
             return {right(),bottom + mu*height};
        }
        point2<T> lerp_top_side(double mu) {
            if (mu < 0.0)
                return {left, top()};
            else if (mu > 1.0)
                return {right(), top()};
            else
             return {left + mu * width,top()};
        }
        point2<T> lerp_bottom_side(double mu) {
            if (mu < 0.0)
                return {left, bottom};
            else if (mu > 1.0)
                return {right(), bottom};
            else
             return {left + mu * width,bottom};
        }
    };

}

#endif //RECT_H 
#define RECT_H