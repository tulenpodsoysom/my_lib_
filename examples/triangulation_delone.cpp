#include "../physics/physical_system/system_geometry.hpp"
#include <fstream>
#include <iostream>

int main()
{

    system_geometry::triangulation_delone<geometry::point2d> t;

    t.generate_square_superstructure(-1, 1, 1, -1);


    std::cout << "hello, world!" << std::endl;
    std::vector<geometry::point2d> nodes(100*100);

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            nodes[i*100 + j] = {-0.75 + 1.5*i/100.0, -0.75 + 1.5*j/100.0 + 0.2*sin(i/100.0)};
        }
    }
    
    auto links = t(nodes.begin(),nodes.end());

    std::ofstream stream("1.txt");
    for (auto& i : links) {
        stream << geometry::point3i(i).to_string<>() << std::endl;
    }

    return 0;
}