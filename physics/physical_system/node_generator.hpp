#include "../../geometry/rect.hpp"
#include <cstddef>
#include <functional>
#include <vector>

struct node_generator2d
{
    geometry::rect<double> node_testing_area;
    size_t width_size;
    size_t height_size;

    std::vector<geometry::point2d>& nodes;

    // evaluates if a node can be placed there
    std::function<bool(double x, double y)> is_acceptable = [](auto x, auto y){return true;};

    void operator() ()
    {
        for (auto i = 0; i < width_size; i++) {
            for (auto j = 0; j < height_size; j++) {
                auto& r = node_testing_area;
                double x = r.left + i*r.width/width_size;
                double y = r.bottom + j*r.height/height_size;
                if (is_acceptable(x,y))
                    nodes.push_back({x,y});
            }
        }
    }
};