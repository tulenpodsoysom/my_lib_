#include "../time_flow_program.hpp"
#include <iostream>

struct parameters
{
    double w0 = 2 * 3.14159265358979323 * 1.0;
    double dt = 0.001;
};

struct variables
{
    double x{0};
    double vx{1};
};

class oscillator : public time_flow_program<parameters, variables>
{
public: 
    
    void cycle_function()
    {
        // x'' = -w0^2 * x;
        v.vx += -p.w0 * p.w0 * v.x * p.dt;
        v.x += p.dt * v.vx;

        if (get_cycle_counter() % 10 == 0)
        {std::cout << "t: " << get_cycle_counter() * p.dt << "\tx: " << v.x << "\tvx:" << v.vx << std::endl;}
    }
};


int main()
{
    oscillator osc;

    osc.start_loop();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    osc.stop_loop();
}

