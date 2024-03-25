#ifndef TRIANGULATION_DELONE_H


#include "../../geometry/geometry.hpp"
#include <algorithm>
#include <initializer_list>
#include <random>
#include <ranges>
#include <set>
#include <utility>
#include <vector>
#include <optional>

#include <iostream>
#include <chrono>
#include <thread>

struct triangulation_delone {
	using point2d = geometry::point2d;

	typedef geometry::circle2dSqrOpt circle_t;
	// struct circle_t {
	// 	point2d center;
	// 	double R2;
	// };

	typedef std::array<int, 3> tri_indexes;

	struct triangulation_data
	{
		tri_indexes indexes{};
		std::optional<circle_t> outer_circle = std::nullopt;
		std::optional<circle_t> inner_circle = std::nullopt;
	};

	std::vector<point2d> superstructure_nodes;

	static auto generate_outer_circle(point2d p1, point2d p2, point2d p3) {
		auto [x1, y1] = p1.as_tuple();
		auto [x2, y2] = p2.as_tuple();
		auto [x3, y3] = p3.as_tuple();

		auto x1_2 = x1 * x1;
		auto y1_2 = y1 * y1;
		auto x2_2 = x2 * x2;
		auto y2_2 = y2 * y2;
		auto x3_2 = x3 * x3;
		auto y3_2 = y3 * y3;

		double det = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
		double det_x = ((x2_2 - x1_2 + y2_2 - y1_2) * (y3 - y1) -
						(x3_2 - x1_2 + y3_2 - y1_2) * (y2 - y1)) /
					   2.0;
		double det_y = ((x3_2 - x1_2 + y3_2 - y1_2) * (x2 - x1) -
						(x2_2 - x1_2 + y2_2 - y1_2) * (x3 - x1)) /
					   2.0;

		circle_t c;
		c.center.x() = det_x / det;
		c.center.y() = det_y / det;

		// Можно избавиться от корня
		c.R2 = ((c.center.x() - x1) * (c.center.x() - x1) +
				(c.center.y() - y1) * (c.center.y() - y1));
		return c;
	}

	static auto generate_inner_circle(point2d p1, point2d p2, point2d p3) {
		double A1 = (p1.y() - p2.y());
		double B1 = (p2.x() - p1.x());
		double C1 = (p1.x() * p2.y() - p2.x() * p1.y());

		double A2 = (p1.y() - p3.y());
		double B2 = (p3.x() - p1.x());
		double C2 = (p1.x() * p3.y() - p3.x() * p1.y());

		double A3 = (p2.y() - p3.y());
		double B3 = (p3.x() - p2.x());
		double C3 = (p2.x() * p3.y() - p3.x() * p2.y());

		double a12 =
			(A1 / sqrt(A1 * A1 + B1 * B1) + A2 / sqrt(A2 * A2 + B2 * B2));
		double b12 =
			(B1 / sqrt(A1 * A1 + B1 * B1) + B2 / sqrt(A2 * A2 + B2 * B2));
		double c12 =
			(C1 / sqrt(A1 * A1 + B1 * B1) + C2 / sqrt(A2 * A2 + B2 * B2));

		double a13 =
			(A1 / sqrt(A1 * A1 + B1 * B1) - A3 / sqrt(A3 * A3 + B3 * B3));
		double b13 =
			(B1 / sqrt(A1 * A1 + B1 * B1) - B3 / sqrt(A3 * A3 + B3 * B3));
		double c13 =
			(C1 / sqrt(A1 * A1 + B1 * B1) - C3 / sqrt(A3 * A3 + B3 * B3));

		double det = a12 * b13 - b12 * a13;
		double det_x = -c12 * b13 + b12 * c13;
		double det_y = -a12 * c13 + c12 * a13;

		double a = geometry::distance(p1, p2), b = geometry::distance(p2, p3),
			   c = geometry::distance(p3, p1);
		double semiperimeter = (a + b + c) / 2.0;
		double R2 = ((semiperimeter - a) * (semiperimeter - b) *
					 (semiperimeter - c) / semiperimeter);
		return circle_t{{det_x / det, det_y / det}, R2};
	};

	// creates rectangle-like superstructure with nodes placed clockwise
	void create_square_superstructure(double l, double r, double t, double b,
										std::random_device &rd) {
		std::uniform_real_distribution<double> dist(-1e-10, 1e-10);
		superstructure_nodes = {point2d({l + dist(rd), t + dist(rd)}),
								point2d({r + dist(rd), t + dist(rd)}),
								point2d({r + dist(rd), b + dist(rd)}),
								point2d({l + dist(rd), b + dist(rd)})};
	}

	template <class T> bool inside_circle(T x, T y, T xc, T yc, T R2) {
		return (x - xc) * (x - xc) + (y - yc) * (y - yc) <= R2;
	}

	static auto encode_index(int a) { return -a - 1; };
	static auto decode_index(int a) { return -a - 1; };
	static auto is_encoded(int a) { return a < 0; };


	// the main usable method
	template <typename InputIt, typename InputIt2>
	std::vector<triangulation_data>
	recursive_enumerate(InputIt begin, InputIt end, InputIt2 begin_super, InputIt2 end_super) {
		auto triang = process_superstructure_nodes(begin_super, end_super);
		for (auto it = begin; it != end; it++) {
			process_new_node(int(it - begin), triang, begin, end, begin_super, end_super);
		}
		delete_superstructure_links(triang);
		return triang;
	}

	template <typename InputIt>
	auto process_superstructure_nodes(InputIt begin_super, InputIt end_super) {
		auto triang = simple_enumerate(begin_super, end_super);

		for (auto &i : triang) {
			for (auto &j : i.indexes) {
				j = encode_index(j);
			}
		}
		return triang;
	}

	template <typename InputIt, typename InputIt2>
	auto process_new_node(int node,
						  std::vector<triangulation_data> &v, InputIt begin,
						  InputIt end, InputIt2 begin_super, InputIt2 end_super) {

		auto a = *(begin + node);
		std::set<int> deleted_triangle_nodes;

		auto pre_end = std::remove_if(v.begin(), v.end(),
					   [&](auto &i) {
						   auto c = i.outer_circle;
						   if (inside_circle(a.x(), a.y(), c->center.x(),
											 c->center.y(), c->R2)) {
							   deleted_triangle_nodes.emplace(i.indexes[0]);
							   deleted_triangle_nodes.emplace(i.indexes[1]);
							   deleted_triangle_nodes.emplace(i.indexes[2]);
							   return true;
						   }
						   return false;
					   });
		v.erase(pre_end, v.end());

		deleted_triangle_nodes.emplace(node);

		std::vector<int> vectorized_node_set(deleted_triangle_nodes.begin(),deleted_triangle_nodes.end());
		auto new_triangles = sub_enumerate(node, vectorized_node_set,begin,end,begin_super,end_super);

		v.insert(v.end(),new_triangles.begin(), new_triangles.end());
	}

	void delete_superstructure_links(std::vector<triangulation_data> &v) {
		auto pre_end = std::remove_if(v.begin(), v.end(), 
		[](auto &a) {return 
			 (a.indexes[0] < 0) || (a.indexes[1] < 0) || (a.indexes[2] < 0);
			 });

		v.erase(pre_end,v.end());
	}

	template <class InputIt>
	auto sub_enumerate(int new_node, std::vector<int> &ind,
					   InputIt begin, InputIt end,
					   InputIt begin_super, InputIt end_super) 
	{
		std::vector<triangulation_data> out;

		for (auto first = ind.begin(); first != ind.end(); first++) {
			for (auto second = first; second != ind.end(); second++) {
				for (auto third = second; third != ind.end(); third++) {
					bool appropriate_circle = true;
					auto p0 =  *(is_encoded(*first)? begin_super + decode_index(*first) : begin +*first);
					auto p1 = *(is_encoded(*second)? begin_super + decode_index(*second) : begin +*second);
					auto p2 =  *(is_encoded(*third)? begin_super + decode_index(*third) : begin +*third);

					auto circle = generate_outer_circle(p0, p1, p2);
					if (isnan(circle.R2))
						continue;

					for (auto i = ind.begin(); i != ind.end(); i++) {
						if (i == first || i == second || i == third)
							continue;

						auto& p = *(is_encoded(*i)? begin_super + decode_index(*i) : begin +*i);

						auto &x = p.x();
						auto &y = p.y();
						auto &x_ = circle.center.x();
						auto &y_ = circle.center.y();
						auto &R2 = circle.R2;

						if (inside_circle(x, y, x_, y_, R2)){
							appropriate_circle = false;
							break;
						}
					}

					if (appropriate_circle) {
						if ((*first == new_node) || (*second == new_node) || (*third == new_node))
						{
							tri_indexes link = {*first,*second,*third};
							out.emplace_back(link, circle);
							break;
						}
					}
				}	
			}
		}
		return out;
	}

	template <class InputIt> auto simple_enumerate(InputIt begin, InputIt end) {
		std::vector<triangulation_data> out;

		for (auto first = begin; first != end; first++) {
			for (auto second = first + 1; second != end; second++) {
				for (auto third = second + 1; third != end; third++) {
					bool appropriate_circle = true;
					auto circle =
						generate_outer_circle(*first, *second, *third);
					if (isnan(circle.R2))
						continue;

					for (auto i = begin; i != end; i++) {
						if (i == first || i == second || i == third)
							continue;

						auto &x = i->x();
						auto &y = i->y();
						auto &x_ = circle.center.x();
						auto &y_ = circle.center.y();
						auto &R2 = circle.R2;

						if ((x - x_) * (x - x_) + (y - y_) * (y - y_) < R2) {
							appropriate_circle = false;
							break;
						}
					}

					if (appropriate_circle) {
						tri_indexes link{int(first - begin),
										 int(second - begin),
										 int(third - begin)};
						out.emplace_back(link,circle,std::nullopt);
						break;
					}
				}
			}
		}
		return out;
	}


	geometry::polygon2d generate_dirichlet_cell(int cell_index, std::vector<triangulation_data>& triang_data)
	{
		// main problem is that I don't know the order of circle centres, so it is hard to reconstruct it

		geometry::polygon2d cell;

		std::vector<triangulation_data*> cell_triangles;

		// finding all triangles what have a main (cell) point
		for (auto& a : triang_data) {
			auto& ind = a.indexes;
			if ((ind[0] == cell_index) || (ind[1] == cell_index) || (ind[2] == cell_index))
			{
				cell_triangles.push_back(std::addressof(a));
			}
		}


		// central point
		geometry::point2d p = std::accumulate(triang_data.begin(),triang_data.end(),geometry::point2d{},
		[](geometry::point2d p , triangulation_data& a) { return p + a.outer_circle->center;});
		p.x() /= triang_data.size();
		p.y() /= triang_data.size();

		// we must go through triangles in a certain order
		std::vector<std::pair<double,int>> angles(cell_triangles.size());
		for (auto l = 0; l < angles.size(); l++) {
			geometry::vector2d v = cell_triangles[l]->outer_circle->center - p;
			angles[l] = {atan2(v.y(),v.x()),l};
		}

		std::sort(angles.begin(),angles.end());

		geometry::polygon2d poly; poly.resize(angles.size());
		for (auto l = 0; l < angles.size(); l++) {
			poly[l] = cell_triangles[angles[l].second]->outer_circle->center;
		}

		return poly;
	}
	template <typename InputIt>
	std::vector<geometry::polygon2d> get_cells(InputIt begin, InputIt end, std::vector<triangulation_data>& triang_data)
	{
		std::vector<geometry::polygon2d> cells(begin - end);
		for (auto i = 0 ; i < cells.size(); i++) {
			cells[i] = generate_dirichlet_cell(i, triang_data);
		}
		return cells;
	}
};

#endif //TRIANGULATION_DELONE_H 
#define TRIANGULATION_DELONE_H