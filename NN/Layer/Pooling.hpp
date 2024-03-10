#pragma once

#include "../Layer.hpp"
#include <cfloat>

class Pooling
{
    size_t pool_h, pool_w;
    pss input_size;

    std::vector<std::vector<pss>> mask;

    size_t y_h, y_w;

public:
    Pooling(){};

    Pooling(const pss &pool, const pss &input)

        : pool_h(pool.first), pool_w(pool.second),
          input_size(input),
          y_h(input.first / pool.first), y_w(input.second / pool.second),
          mask(input.first / pool.first, std::vector<pss>(input.second / pool.second))
    {
    }

    size_t get_y_h() { return y_h; }
    size_t get_y_w() { return y_w; }

    vvd forward(const vvd &x)
    {
        vvd y(y_h, vd(y_w, -DBL_MAX));

        for (size_t i = 0; i < y_h; ++i)
        {
            for (size_t j = 0; j < y_w; ++j)
            {
                for (size_t m = 0; m < pool_h; ++m)
                {
                    for (size_t n = 0; n < pool_w; ++n)
                    {
                        size_t row = i * pool_h + m;
                        size_t col = j * pool_w + n;
                        if (y[i][j] < x[row][col])
                        {
                            y[i][j] = x[row][col];
                            mask[i][j] = std::make_pair(row, col);
                        }
                    }
                }
            }
        }
        return y;
    }

    vvd backward(const vvd &dy)
    {
        vvd dx(input_size.first, vd(input_size.second));

        for (size_t i = 0; i < y_h; ++i)
        {
            for (size_t j = 0; j < y_w; ++j)
            {
                dx[mask[i][j].first][mask[i][j].second] = dy[i][j];
            }
        }
        return dx;
    }
};

class Pool : public Layer
{
    Pooling P;

    pss pool;
    size_t X;

public:
    Pool(pss pool, tsss input_size = {0, 0, 0})

        : pool(pool)
    {
        if (std::get<0>(input_size) != 0 and std::get<1>(input_size) != 0 and std::get<2>(input_size) != 0)
        {
            set(input_size);
        }
    }

    void set(const tsss &input_size) override
    {
        X = std::get<0>(input_size);
        P = Pooling(pool, {std::get<1>(input_size), std::get<2>(input_size)});
    }

    tsss get() override { return {X, P.get_y_h(), P.get_y_w()}; }

    vvvd forward(const vvvd &x) override
    {
        vvvd y(x.size());

        for (size_t i = 0; i < x.size(); ++i)
        {
            y[i] = P.forward(x[i]);
        }
        return y;
    }

    vvvd backward(const vvvd &dy) override
    {
        vvvd dx(dy.size());

        for (size_t i = 0; i < dy.size(); ++i)
        {
            dx[i] = P.backward(dy[i]);
        }
        return dx;
    }
};