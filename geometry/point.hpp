#ifndef POINT_H

#include <array>
#include <cmath>
#include <functional>
#include <numeric>
#include <string>

namespace geometry {
template <class a, class func>
constexpr a element_wise_operation(a p1, a p2, func f) {
	a p = p1;
	for (auto i = 0; i < p1.size(); i++) {
		p[i] = f(p1[i], p2[i]);
	}
	return p;
}

template <class Point> constexpr Point add(Point p1, Point p2) {
	return element_wise_operation(p1, p2, std::plus<typename Point::Type>{});
}
template <class Point> constexpr Point subtract(Point p1, Point p2) {
	return element_wise_operation(p1, p2, std::minus<typename Point::Type>{});
}
template <class Point> constexpr Point multiply(Point p1, Point p2) {
	return element_wise_operation(p1, p2,
								  std::multiplies<typename Point::Type>{});
}
template <class Point> constexpr Point divide(Point p1, Point p2) {
	return element_wise_operation(p1, p2, std::divides<typename Point::Type>{});
}
template <class Point> constexpr Point middle_point(Point p1, Point p2) {
	return element_wise_operation(
		p1, p2, [](auto &a, auto &b) { return (a + b) / 2.0; });
}

template <class Point> constexpr Point middle_point(std::initializer_list<Point> l)
{
	Point middle(
		std::accumulate(l.begin(),l.end(),0.0,[](double a, Point b) {return a + b.x();}) / l.size(),
		std::accumulate(l.begin(),l.end(),0.0,[](double a, Point b) {return a + b.y();}) / l.size()
	);
	return middle;
}

template <class InputIt, class Point> Point middle_point(InputIt begin, InputIt end) 
{
	Point middle(
		std::accumulate(begin,end,0.0,[](double a, Point b) {return a + b.x();}) / double(begin - end),
		std::accumulate(begin,end,0.0,[](double a, Point b) {return a + b.y();}) / double(begin - end)
	);
	return middle;
}

template <class Point> constexpr double distance2(Point p1, Point p2) {
	double d = 0;
	for (auto i = 0; i < p1.size(); i++) {
		d += (p1[i] - p2[i]) * (p1[i] - p2[i]);
	}
	return d;
}

template <class Point> constexpr double distance(Point p1, Point p2) {
	return std::sqrt(distance2(p1, p2));
}

template <class Point> constexpr double abs2(Point p) {
	double d = 0;
	for (auto i = 0; i < p.size(); i++) {
		d += p[i] * p[i];
	}
	return d;
}
template <class Point> constexpr double abs(Point p) {
	return std::sqrt(abs2(p));
}

template <class Point> constexpr Point lerp(Point p1, Point p2, double mu)
{
	return element_wise_operation(p1, p2, [&mu](auto& a,auto& b){return a + (b - a)*mu;});
}



template <size_t dimensions, typename T>
struct point : public std::array<T, dimensions> {
	using Base = std::array<T, dimensions>;
	using Base::Base;
	using Base::operator[];
	using Base::operator=;
	using Type = T;

	point<dimensions, T> operator+(point<dimensions, T> p) {
		return add(*this, p);
	}

	point<dimensions, T> operator-(point<dimensions, T> p) {
		return subtract(*this, p);
	}

	point<dimensions, T> operator*(point<dimensions, T> p) {
		return multiply(*this, p);
	}

	point<dimensions, T> operator/(point<dimensions, T> p) {
		return divide(*this, p);
	}

	std::string to_string() {
		if constexpr (dimensions == 0)
			return std::string("{}");

		std::string s = "{";
		for (auto i = 0; i < dimensions - 1; i++) {
			s += std::to_string(this->at(i));
			s += ',';
		}
		s += std::to_string(this->at(dimensions - 1));
		s += "}";
		return s;
	};
	std::wstring to_wstring() {
		if constexpr (dimensions == 0)
			return std::wstring(L"{}");

		std::wstring s = L"{";
		for (auto i = 0; i < dimensions - 1; i++) {
			s += std::to_wstring(this->at(i));
			s += L',';
		}
		s += std::to_wstring(this->at(dimensions - 1));
		s += L"}";
		return s;
	};
};

template <typename T> struct point1 : public std::array<T, 1> {
	point1<T>(T x = 0) : std::array<T, 1>{x} {}
	point1<T> &operator=(point1<T> p) {
		(*this)[0] = p[0];
		return (*this);
	}
	using Base = std::array<T, 1>;
	using Base::Base;
	using Base::operator[];
	//using Base::operator=;
	using Type = T;

	T &x (){ return this->at(0);}

	point1<T> operator+(point1<T> p) { return add(*this, p); }

	point1<T> operator-(point1<T> p) { return subtract(*this, p); }

	point1<T> operator*(point1<T> p) { return multiply(*this, p); }

	point1<T> operator/(point1<T> p) { return divide(*this, p); }

	std::string to_string() { return "{" + std::to_string(x()) + "}"; }
	std::wstring to_wstring() { return L"{" + std::to_wstring(x()) + L"}"; }

	std::tuple<T&> as_tuple(){return {x()};}
};

template <typename T> struct point2 : public std::array<T, 2> {
	point2<T>(T x = 0, T y = 0) : std::array<T, 2>{x, y} {}
	//point2<T>(point2 &p) : point2<T>(p.x, p.y) {}
	point2<T> &operator=(point2<T> p) {
		(*this)[0] = p[0];
		(*this)[1] = p[1];
		return (*this);
	}

	using Base = std::array<T, 2>;
	using Base::Base;
	using Base::operator[];
	//using Base::operator=;
	using Type = T;

	T &x (){ return this->at(0);}
	T &y (){ return this->at(1);}

	point2<T> operator+(point2<T> p) { return add(*this, p); }

	point2<T> operator-(point2<T> p) { return subtract(*this, p); }

	point2<T> operator*(point2<T> p) { return multiply(*this, p); }

	point2<T> operator/(point2<T> p) { return divide(*this, p); }

	std::string to_string() {
		return "{" + std::to_string(x()) + ", " + std::to_string(y()) + "}";
	}
	std::wstring to_wstring() {
		return L"{" + std::to_wstring(x()) + L", " + std::to_wstring(y()) + L"}";
	}
	std::tuple<T&,T&> as_tuple(){return {x(),y()};}
};

template <typename T> struct point3 : public std::array<T, 3> {
	
	point3<T>(T x = 0, T y = 0, T z = 0) : std::array<T, 3>{x, y, z} {}
	point3<T> &operator=(point3<T> p) {
		(*this)[0] = p[0]; 
		(*this)[1] = p[1]; 
		(*this)[2] = p[2]; 
		return (*this);
	}
	//point3<T>(point3<T> &p) : point3<T>(p.x, p.y, p.z) {}
	using Base = std::array<T, 3>;
	using Base::Base;
	using Base::operator[];
	//using Base::operator=;
	using Type = T;

	T &x (){ return this->at(0);}
	T &y (){ return this->at(1);}
	T &z (){ return this->at(2);}

	point3<T> operator+(point3<T> p) { return add(*this, p); }

	point3<T> operator-(point3<T> p) { return subtract(*this, p); }

	point3<T> operator*(point3<T> p) { return multiply(*this, p); }

	point3<T> operator/(point3<T> p) { return divide(*this, p); }

	std::string to_string()  {
		return "{" + std::to_string(x()) + ", " + std::to_string(y()) + ", " +
			   std::to_string(z()) + "}";
	}
	std::wstring to_wstring()  {
		return L"{" + std::to_wstring(x()) + L", " + std::to_wstring(y()) + L", " +
			   std::to_wstring(z()) + L"}";
	}

	std::tuple<T&,T&,T&> as_tuple(){return {x(),y(),z()};}
};

template <typename T> std::string to_string(point1<T> p) {
	return p.to_string();
}
template <typename T> std::string to_string(point2<T> p) {
	return p.to_string();
}
template <typename T> std::string to_string(point3<T> p) {
	return p.to_string();
}

template <typename T> std::wstring to_wstring(point1<T> p) {
	return p.to_wstring();
}
template <typename T> std::wstring to_wstring(point2<T> p) {
	return p.to_wstring();
}
template <typename T> std::wstring to_wstring(point3<T> p) {
	return p.to_wstring();
}

template <typename T>
std::ostream &operator<<(std::ostream &stream, point1<T>& p) {
	stream << p.to_string(); return stream;
}
template <typename T>
std::ostream &operator<<(std::ostream &stream, point2<T>& p) {
	stream << p.to_string(); return stream;
}
template <typename T>
std::ostream &operator<<(std::ostream &stream, point3<T>& p) {
	stream << p.to_string(); return stream;
}

using point3i = point3<int>;
using point3u = point3<unsigned>;
using point3f = point3<float>;
using point3d = point3<double>;

using point2i = point2<int>;
using point2u = point2<unsigned>;
using point2f = point2<float>;
using point2d = point2<double>;

using point1i = point1<int>;
using point1u = point1<unsigned>;
using point1f = point1<float>;
using point1d = point1<double>;

using point3i = point3<int>;
using point3u = point3<unsigned>;
using point3f = point3<float>;
using point3d = point3<double>;

using point2i = point2<int>;
using point2u = point2<unsigned>;
using point2f = point2<float>;
using point2d = point2<double>;

using vector1i = point1i;
using vector1u = point1u;
using vector1f = point1f;
using vector1d = point1d;

using vector2i = point2i;
using vector2u = point2u;
using vector2f = point2f;
using vector2d = point2d;

using vector3i = point3i;
using vector3u = point3u;
using vector3f = point3f;
using vector3d = point3d;

// static auto a = sizeof(point1d)
} // namespace geometry

#endif //POINT_H 
#define POINT_H