#pragma once

#include "out.hpp"

#include <chrono>

class measure
{
    std::chrono::system_clock::time_point start, end;

public:
    measure()
    {
        start = std::chrono::system_clock::now();
    }
    ~measure()
    {
        end = std::chrono::system_clock::now();
        double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
        out("time:", time, "[ms]");
        newline();
    }
};