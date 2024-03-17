#include <functional>
namespace geometry {
template <typename T> struct point1 {
	T x{};
};
template <typename T> struct point2 {
	T x{}, y{};
};
template <typename T> struct point3 {
	T x{}, y{}, z{};
};

// template <typename T, typename Func>
// auto element_wise_operation(point1<T> p1, point1<T> p2, Func f) {
// 	return point1<T>(f(p1.x, p2.x));
// }

// template <typename T, typename Func>
// auto element_wise_operation(point2<T> p1, point2<T> p2, Func f) {
// 	return point2<T>(f(p1.x, p2.x), f(p1.y, p2.y));
// }

// template <typename T, typename Func>
// auto element_wise_operation(point3<T> p1, point3<T> p2, Func f) {
// 	return point3<T>(f(p1.x, p2.x), f(p1.y, p2.y), f(p1.z, p2.z));
// }

// template <typename T> inline auto operator+(point1<T> a, point1<T> b) {
// 	return element_wise_operation(a, b, std::plus<T>());
// }
// template <typename T> inline auto operator-(point1<T> a, point1<T> b) {
// 	return element_wise_operation(a, b, std::minus<T>());
// }
// template <typename T> inline auto operator*(point1<T> a, point1<T> b) {
// 	return element_wise_operation(a, b, std::multiplies<T>());
// }
// template <typename T> inline auto operator/(point1<T> a, point1<T> b) {
// 	return element_wise_operation(a, b, std::divides<T>());
// }

// template <typename T> inline auto operator+(point2<T> a, point2<T> b) {
// 	return element_wise_operation(a, b, std::plus<T>());
// }
// template <typename T> inline auto operator-(point2<T> a, point2<T> b) {
// 	return element_wise_operation(a, b, std::minus<T>());
// }
// template <typename T> inline auto operator*(point2<T> a, point2<T> b) {
// 	return element_wise_operation(a, b, std::multiplies<T>());
// }
// template <typename T> inline auto operator/(point2<T> a, point2<T> b) {
// 	return element_wise_operation(a, b, std::divides<T>());
// }

// template <typename T> inline auto operator+(point3<T> a, point3<T> b) {
// 	return element_wise_operation(a, b, std::plus<T>());
// }
// template <typename T> inline auto operator-(point3<T> a, point3<T> b) {
// 	return element_wise_operation(a, b, std::minus<T>());
// }
// template <typename T> inline auto operator*(point3<T> a, point3<T> b) {
// 	return element_wise_operation(a, b, std::multiplies<T>());
// }
// template <typename T> inline auto operator/(point3<T> a, point3<T> b) {
// 	return element_wise_operation(a, b, std::divides<T>());
// }
} // namespace geometry