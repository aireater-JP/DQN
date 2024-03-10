#pragma once
#include "../Layer.hpp"

class ReLU : public Layer
{
    vvvd mask;

    tsss input_size;

public:
    void set(const tsss &input) override { input_size = input; };
    tsss get() override { return input_size; }

    vvvd forward(const vvvd &x) override
    {
        mask = x;
        vvvd y(x.size(), vvd(x[0].size(), vd(x[0][0].size())));

        for (size_t i = 0; i < x.size(); ++i)
        {
            for (size_t j = 0; j < x[i].size(); ++j)
            {
                for (size_t k = 0; k < x[i][j].size(); ++k)
                {
                    y[i][j][k] = std::max(0.0, x[i][j][k]);
                }
            }
        }
        return y;
    }

    vvvd backward(const vvvd &dy) override
    {
        vvvd dx(dy.size(), vvd(dy[0].size(), vd(dy[0][0].size())));

        for (size_t i = 0; i < dy.size(); ++i)
        {
            for (size_t j = 0; j < dy[i].size(); ++j)
            {
                for (size_t k = 0; k < dy[i][j].size(); ++k)
                {
                    dx[i][j][k] = dy[i][j][k] * (mask[i][j][k] > 0.0);
                }
            }
        }
        return dx;
    }
};