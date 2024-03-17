#include "../PlotImage.h"
#include "../FFT.hpp"


// In order to compile use this:
// g++ .\plot_example.cpp ..\PlotImage.cpp -lgdiplus -lole32

int main()
{
    const unsigned N = 1024*8;
    std::vector<double> signal(N);
    std::vector<std::complex<double>> signal_c(N);
    for (size_t i = 0; i < N; i++)
    {
        const double PI = 3.14159265358979323;
        signal[i]   = round(fmod(25*i/(double)N,1.0))- 0.5;
        signal_c[i] = signal[i];
    }
    fourier(signal_c.begin(),signal_c.end(),signal_c.begin(),-1);

    my_graphics::PlotImage plot;

    plot.set_points(signal.begin(),signal.end());
    plot.save_as_file(L"output_signal.png",1280,720);

    abs_transform(signal_c.begin(),signal_c.end(),signal.begin());
    plot.acc_settings.bounds_detection_x = plot.acc_settings.max_min;
    plot.acc_settings.accommodate_grid_scale = false;
    plot.set_points(signal.begin(),signal.end(),N);
    plot.save_as_file(L"output_fourier.png",1920,1080);

}