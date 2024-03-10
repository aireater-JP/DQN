#pragma once

#include "../Random.hpp"

#include "../Layer.hpp"
#include <cmath>

// シグモイドとか
const int Xavier = 0;
// ReLUとか
const int He = 1;

class Dense : public Layer
{
    vvd W;
    vvd dW;
    vd B;
    vd dB;

    vvvd input;

    size_t output_size;
    int Init_type;

public:
    Dense(size_t output_size, int Init_type = Xavier, size_t input_size = 0)
        : B(output_size),
          dB(output_size),
          output_size(output_size),
          Init_type(Init_type)
    {
        if (input_size != 0)
        {
            set({1, 1, input_size});
        }
    }

    void set(const tsss &input_size) override
    {
        size_t Z = std::get<2>(input_size);

        W = vvd(Z, vd(output_size));
        dW = vvd(Z, vd(output_size));

        Random<std::normal_distribution<>> r(0.0, 0.0);

        if (Init_type == Xavier)
        {
            r.set(0.0, 1.0 / std::sqrt(Z));
        }
        if (Init_type == He)
        {
            r.set(0.0, std::sqrt(2.0 / Z));
        }

        for (size_t i = 0; i < Z; ++i)
        {
            for (size_t j = 0; j < output_size; ++j)
            {
                W[i][j] = r();
            }
        }
    }

    tsss get() override { return {1, 1, output_size}; }

    vvvd forward(const vvvd &x) override
    {
        input = x;
        vvvd y(1, vvd(1, B));
        for (size_t i = 0; i < W.size(); ++i)
        {
            for (size_t j = 0; j < W[i].size(); ++j)
            {
                y[0][0][j] += W[i][j] * x[0][0][i];
            }
        }
        return y;
    }

    vvvd backward(const vvvd &dy) override
    {
        vvvd dx(1, vvd(1, vd(W.size())));
        for (size_t i = 0; i < W.size(); ++i)
        {
            for (size_t j = 0; j < W[i].size(); ++j)
            {
                dx[0][0][i] += W[i][j] * dy[0][0][j];
                dW[i][j] += input[0][0][i] * dy[0][0][j];
            }
        }

        dB = add(dB, dy[0][0]);

        return dx;
    }

    void update(const double lr) override
    {
        dB = mul(dB, -lr);
        B = add(B, dB);
        clean(dB);

        dW = mul(dW, -lr);
        W = add(W, dW);
        clean(dW);
    }

    void clear()override
    {
        clean(dB);
        clean(dW);
    }
};