#ifndef CIRCLE_H
#include "point.hpp"

namespace geometry {
static const double PI = 3.14159265358979323;
struct circle2d {
	point2d center;
	double radius;

	bool contains(double x, double y) {
		return distance2(center, {x, y}) < radius * radius;
	}
	bool contains(point2d p) { return distance2(center, p) < radius * radius; }

	double area() { return PI * radius * radius; }
	double circumference() { return 2.0 * PI * radius; }
	double diameter() { return 2.0 * radius; }
};

struct circle3d {
	point3d center;
	double radius;

	bool contains(double x, double y, double z) {
		return distance2(center, {x, y, z}) < radius * radius;
	}
	bool contains(point3d p) { return distance2(center, p) < radius * radius; }

	double area() { return 4.0 * PI * radius * radius; }
	double diameter() { return 2.0 * radius; }
	double volume() {return 4.0/3.0 * PI * radius*radius*radius;}
};



// For optimization purposes this class doesn't require knowing it's radius, only square of it.
// It prevents the use of sqrt(...) to calculate the radius
struct circle2dSqrOpt
{
	point2d center;
	double R2;

	bool contains(double x, double y) {
		return distance2(center, {x, y}) < R2;
	}
	bool contains(point2d p) { return distance2(center, p) < R2; }

	double area() { return PI * R2; }
	double circumference() { return 2.0 * PI * sqrt(R2); }
	double diameter() { return 2.0 * sqrt(R2); }
	double radius() { return sqrt(R2); }
};

// For optimization purposes this class doesn't require knowing it's radius, only square of it.
// It prevents the use of sqrt(...) to calculate the radius
struct circle3dSqrOpt {
	point3d center;
	double R2;

	bool contains(double x, double y, double z) {

		return distance2(center, {x, y, z}) < R2;
	}
	bool contains(point3d p) { return distance2(center, p) < R2; }

	double area() { return 4.0 * PI * R2; }
	double diameter() { return 2.0 * sqrt(R2); }
	double volume() {return 4.0/3.0 * PI * pow(R2,3./2.);}
	double radius() {return sqrt(R2);}
};


} // namespace geometry

#endif //CIRCLE_H 
#define CIRCLE_H