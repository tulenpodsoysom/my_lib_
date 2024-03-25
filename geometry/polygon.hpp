#ifndef POLYGON_H
#include "point.hpp"
#include "line.hpp"

namespace geometry {


struct polygon2d : public std::vector<point2d>
{
    using std::vector<point2d>::operator[];
    using std::vector<point2d>::vector;

    bool contains(point2d p)
    {
        unsigned intersections = 0;

		for (auto i = 0; i < size() - 1; i++) {

			auto &p1 = at(i);
			auto &p2 = at(i + 1);

			bool xpos1 = (p1-p).x() > 0 ? true : false;
			bool ypos1 = (p1-p).y() > 0 ? true : false;
			bool xpos2 = (p2-p).x() > 0 ? true : false;
			bool ypos2 = (p2-p).y() > 0 ? true : false;

            // if points are in one quadrant
            if ((xpos1 == xpos2) && (ypos1 == ypos2)) continue;

            // if points are in left semiplane
            if ((xpos1 == false) && (xpos2 == false)) continue;

			line2d l{p1, p2};

            // 0 * x + 1 * y + -p.y = 0.
            // y = p.y
			line2d x;
			x.A = 0;
			x.B = 1;
			x.C = -p.y();

			if (l.intersection(x).x() < 0)
				intersections++;
		}
		return (intersections % 2 == 1) ? true : false;
	}

};

}
#endif //POLYGON_H 
#define POLYGON_H