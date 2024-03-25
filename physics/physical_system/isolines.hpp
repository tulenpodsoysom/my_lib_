#include "../../geometry/geometry.hpp"
#include "triangulation_delone.hpp"
#include <algorithm>


// uses triangulation data
struct isoline_generator
{
    unsigned levels{10};

    std::vector<double>& node_values;
    std::vector<geometry::point2d>& nodes;
    std::vector<triangulation_delone::triangulation_data>& triang_data;

    std::vector<std::pair<geometry::point2d,geometry::point2d>> get_isolines()
    {
        if (nodes.size() < 0) return {};
        if (node_values.size() < 0) return {};

        std::vector<std::pair<geometry::point2d,geometry::point2d>> isolines;
		auto [min,max] = std::minmax_element(node_values.begin(), node_values.end());

        for (auto l = 0 ; l < levels; l++) {
            double level = *min + (*max - *min) * l/double(levels);

            for (auto& d : triang_data) {
                geometry::point3d points[3] =
                {
                    {nodes[d.indexes[0]].x(),nodes[d.indexes[0]].y(),node_values[d.indexes[0]]},
                    {nodes[d.indexes[1]].x(),nodes[d.indexes[1]].y(),node_values[d.indexes[1]]},
                    {nodes[d.indexes[2]].x(),nodes[d.indexes[2]].y(),node_values[d.indexes[2]]}
                };

                std::sort(points,points + 3,[](auto& a, auto& b) {return a.z() < b.z();});
				if ((level < points[0].z()) || (level > points[2].z())) continue;
                bool upper_than_middle_one = level > points[1].z();

                // if Tc above the second point then it intersects (p2~p1 and p2~p0) else (p0~p1 and p0~p2)
                geometry::point3d& common_p = (upper_than_middle_one)? points[2] : points[0];
                geometry::point3d& p_1 = points[1];
                geometry::point3d& p_2 = (upper_than_middle_one)? points[0] : points[2];


                // T2-Tc/T2-T1 = x2-xc/x2-x1
                double T_factor = (common_p.z() - level)/(common_p.z() - p_1.z());

                geometry::point2d inter_1 = {
                    -(common_p.x() - p_1.x())*T_factor + common_p.x(),
                    -(common_p.y() - p_1.y())*T_factor + common_p.y(),
                    };

                T_factor = (common_p.z() - level)/(common_p.z() - p_2.z());
                geometry::point2d inter_2 = {
                    -(common_p.x() - p_2.x())*T_factor + common_p.x(),
                    -(common_p.y() - p_2.y())*T_factor + common_p.y(),
                };

                isolines.push_back({inter_1,inter_2});
			}
        }
        return isolines;
	}
};