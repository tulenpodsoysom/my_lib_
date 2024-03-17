#include <vector>

template <typename T>
struct color
{
    T R,G,B;
};


template <typename T>
struct image
{
    std::vector<std::vector<color<T>>> pixels;

    image(unsigned width, unsigned height):
    pixels(height,std::vector<T>(width)){}

    T& at(unsigned i, unsigned j)
    {
        if ((i > pixels.size()) || (j > pixels[0].size())) return {}; //!!!
        return pixels[i][j];
    }
};



