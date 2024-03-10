#pragma once

#include "utility.hpp"

class Loss
{
public:
    virtual double forward(const vvvd &x, const vd &t) = 0;
    virtual vvvd backward() = 0;

    virtual void save(fout &f) = 0;
};

#include "Loss/Softmax.hpp"
#include "Loss/Identity.hpp"