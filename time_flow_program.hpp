#pragma once
#include <mutex>
#include <thread>
#include <iostream>


template <class parameters, class variables>
class time_flow_program
{
protected:
    bool do_cycle = false; unsigned cycle_counter = 0;

public:
    std::ostream& logger_stream = std::cout;

    parameters p;
    variables  v;

    std::mutex variables_mutex; 


    //--- Используется для первичных задач, формирование исходного сигнала и т.п.

    //virtual void init(const parameters params) { p = params;};
    virtual void init() {};
    virtual void prepare() {};
    //--- Используется для исполнения параллельных задач (убрать private если нужно)
//private:
    std::thread loop_thread;
    virtual void cycle_function() = 0;
    virtual void loop_function()
    {
        while (do_cycle)
        {
            cycle_function();
            cycle_counter++;
        }
    };
    void start_loop()
    {
        cycle_counter = 0;
        do_cycle = true; 
        loop_thread = std::thread([this]{loop_function();});
    };
    void stop_loop(){ do_cycle = false; loop_thread.join(); };

    unsigned get_cycle_counter() {return cycle_counter;};
    //---
public:
    bool running() {return do_cycle;}
    void clear() {v = {};};
};


