#include <functional>

template<size_t size>
struct image_mask : std::array<std::array<double,size>,size>;
{
    
};



void mask_value(
    unsigned i, unsigned j,
    unsigned bitmap_width,
    unsigned bitmap_height,
    unsigned mask_width,
    unsigned mask_height,
    std::function<unsigned(unsigned, unsigned)> access_func,
    std::function<unsigned(unsigned, unsigned)> mask_access_func
    )
{

}


