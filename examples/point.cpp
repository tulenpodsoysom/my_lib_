#include "../geometry/point.hpp"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace geometry;
using namespace std;

int main() {
	try {
		point1i p1i = {3};
		point1u p1u = {1};
		point1f p1f = {3};
		point1d p1d = {1};

		point2i p2i = {3, 5};
		point2u p2u = {1, 6};
		point2f p2f = {3.4f,5.2f};
		point2d p2d = {1.2, 6.8};

		point3i p3i = {3, 5, 9};
		point3u p3u = {1, 6, 3};
		point3f p3f = {3.4f, 5.2f, 9.9f};
		point3d p3d = {1.2, 6.8, 3.8};

		cout << "String parsing test" << endl;

		cout << "p1i: " << p1i.to_string() << endl;
		cout << "p1u: " << p1u.to_string() << endl;
		cout << "p1f: " << p1f.to_string() << endl;
		cout << "p1d: " << p1d.to_string() << endl;

		cout << "p2i: " << p2i.to_string() << endl;
		cout << "p2u: " << p2u.to_string() << endl;
		cout << "p2f: " << p2f.to_string() << endl;
		cout << "p2d: " << p2d.to_string() << endl;

		cout << "p3i: " << p3i.to_string() << endl;
		cout << "p3u: " << p3u.to_string() << endl;
		cout << "p3f: " << p3f.to_string() << endl;
		cout << "p3d: " << p3d.to_string() << endl;
		cout << endl;

		cout << "testing variable access p3d.x(), p3d.y(), p3d.z():\n" << p3d.x() << ' ' << p3d.y()
			 << ' ' << p3d.z() << endl;
		cout << "testing ostream operator (p3d):\n" << p3d << endl << endl;
		cout << "testing vector creation" << endl;

		auto print_first_n = [&](auto &v, int n) {
			for (int i = 0; i < n; i++) {
				cout << v[i] << endl;
			}
		};

		cout << "std::generate " << endl;
		random_device rd;
		uniform_real_distribution<> dist(-1, 1);
		vector<point3d> v;
		v.resize(100);
		generate(v.begin(), v.end(), [&]() {
			return point3d{dist(rd), dist(rd), dist(rd)};
		});
		print_first_n(v, 2);

		cout << "emplace_back " << endl;
		v = {};
		for (auto i = 0; i < 100; i++) {
			v.emplace_back(dist(rd), dist(rd), dist(rd));
		}
		print_first_n(v, 2);

		cout << "push_back " << endl;
		v = {};
		for (auto i = 0; i < 100; i++) {
			v.push_back({dist(rd), dist(rd), dist(rd)});
		}
		print_first_n(v, 2);


        cout << "testing structural binding, using\n\tauto[x,y,z] = p3d.as_tuple()" << endl;
        {
            auto [x,y,z] = p3d.as_tuple();
			printf("getting x,y,z \t{%f, %f, %f}\n", x, y, z);
			printf("changing x,y,z \t{%f, %f, %f}\n", ++x, ++y, ++z);
            cout << "observing p3d\t" << p3d << endl;
        }

        cout << "\ntesting arithmetic operations" << endl;
		cout << "addition \t" << (p3d + p3d).to_string() << endl;
		cout << "subtraction \t" << (p3d - p3d).to_string() << endl;
		cout << "multiply \t" << (p3d * p3d).to_string() << endl;
		cout << "division \t" << (p3d / p3d).to_string() << endl;

		//cout << endl;
		//cout << "distance \t" << geometry::distance(p3d, p3f) << endl;
		//cout << "distance_2 \t" << geometry::distance2(p31, p32) << endl;
		//cout << "midpoint \t" << geometry::middle_point(p31, p32).to_string()
		//	 << endl;
	} catch (std::exception e) {
		cout << e.what() << endl;
	}
}