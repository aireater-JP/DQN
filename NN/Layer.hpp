#pragma once

#include "utility.hpp"
#include "fio.hpp"

class Layer
{
public:
    virtual tsss get() = 0;
    virtual void set(const tsss &input_size) = 0;

    virtual vvvd forward(const vvvd &x) = 0;

    virtual vvvd backward(const vvvd &dy) = 0;

    virtual void update(const double lr) {}

    virtual void save(fout &f) = 0;
};

#include "Layer/Convolutional2d.hpp"
#include "Layer/Dense.hpp"
#include "Layer/Flatten.hpp"
#include "Layer/Pooling.hpp"
#include "Layer/ReLU.hpp"
#include "Layer/Sigmoid.hpp"