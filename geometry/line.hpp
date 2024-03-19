#ifndef LINE_H


#include <cmath>

//  Describes a primitive line equation
//  A * x + B * y + C = 0

struct line
{
    double A{},B{},C{};

    // line passing through two points
    line(double x1, double y1, double x2, double y2, bool normalized = false)
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

	line(double x, double y, double radians) {
        static const double PI = 3.14159265358979323;
        A = cos(radians + PI/2.0);
        B = sin(radians + PI/2.0);

        C = -(A*x + B*y);
    }

    //returns A*x + B*y + C;
    double operator() (double x , double y) {return A*x + B*y + C;}

    //static double get_A(double x1, double y1, double x2, double y2)
    //{return -1.0/(x1-x2);}
    //static double get_B(double x1, double y1, double x2, double y2)
    //{return 1.0/(y1-y2);}
    //static double get_C(double x1, double y1, double x2, double y2)
    //{return -(x1/(x1-x2) + y1/(y1-y2));}
};

#endif //LINE_H 
#define LINE_H