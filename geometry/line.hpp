#ifndef LINE_H
#include "point.hpp"

namespace geometry {


//  Describes a primitive line equation
//  A * x + B * y + C = 0

struct line2d
{
    double A{},B{},C{};

    line2d(){};
    line2d(point2d p1, point2d p2) : line2d(p1.x(),p1.y(),p2.x(),p2.y()){};
    // line passing through two points
    line2d(double x1, double y1, double x2, double y2, bool normalized = false)
    {
        A = (y1-y2); //if (isinf(A)){ A = 1.0; B = 0.0; C = -A;}
        B = (x2-x1); //if (isinf(B)){ A = 0.0; B = 1.0; C = -B;}
        
		C = (x1*y2 - x2*y1);
        if (normalized)
        {
            double abs = sqrt(A*A+B*B);
            A /= abs;
            B /= abs;
            C /= abs;
        }
    }

	line2d(double x, double y, double radians) {
        static const double PI = 3.14159265358979323;
        A = cos(radians + PI/2.0);
        B = sin(radians + PI/2.0);

        C = -(A*x + B*y);
    }

    //returns A*x + B*y + C;
    double operator() (double x , double y) {return A*x + B*y + C;}


	point2d intersection(line2d l2) {
        auto& l1 = (*this);
		double det = l1.A * l2.B - l1.B * l2.A;
		double det_x = -l1.C * l2.B + l1.B * l2.C;
		double det_y = -l1.A * l2.C + l1.C * l2.A;

		return {det_x / det, det_y / det};
	}
};

}

#endif //LINE_H 
#define LINE_H