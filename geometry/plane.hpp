#include "point.hpp"

namespace geometry
{
    struct plane3d
    {
        double A{},B{},C{},D{};

		plane3d(double A, double B, double C, double D)
			: A(A), B(B), C(C), D(D){};

		plane3d(point3d p1, point3d p2, point3d p3) { 
            double a, b, c;
			a = (p2.y - p1.y) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.y - p1.y);
            b = (p2.x - p1.x) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.x - p1.x);
            c = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);

            A = a; B = -b; C = c; D = -p1.x*a + p1.y*b - p1.z*c;
		}

		plane3d(point3d origin, vector3d normal)
			: A(normal.x), B(normal.y), C(normal.z),
			  D(-abs2(origin * normal)){};
	};
}