#pragma once

#include "Convolutional.hpp"
#include "../Layer.hpp"

// ちゃんねるを管理するクラス
class Conv_cell
{
    std::vector<Convolutional> Conv;

    size_t y_h, y_w;

public:
    Conv_cell(const size_t channel,
              const pss &input_size,
              const pss &filter,
              const pss &stride = {1, 1},
              const pss &padding = {0, 0})

        : Conv(channel, Convolutional(input_size, filter, stride, padding)),

          y_h(Conv[0].get_y_h()),
          y_w(Conv[0].get_y_w())
    {
    }

    size_t get_y_h() { return y_h; }
    size_t get_y_w() { return y_w; }

    vvd forward(const vvvd &x)
    {
        vvd y(y_h, vd(y_w));

        for (size_t i = 0; i < Conv.size(); ++i)
        {
            y = add(y, Conv[i].forward(x[i]));
        }
        return y;
    }

    vvvd backward(const vvd &dy)
    {
        vvvd dx(Conv.size());

        for (size_t i = 0; i < Conv.size(); ++i)
        {
            dx[i] = Conv[i].backward(dy);
        }
        return dx;
    }

    void update(const double lr)
    {
        for (size_t i = 0; i < Conv.size(); ++i)
        {
            Conv[i].update(lr);
        }
    }

    void clear()
    {
        for (size_t i = 0; i < Conv.size(); ++i)
        {
            Conv[i].clear();
        }
    }
};

class Conv2d : public Layer
{
    vd B;
    vd dB;
    std::vector<Conv_cell> Conv;

    size_t filter_num;
    size_t X, Y, Z;

    pss filter;
    pss stride;
    pss padding;

public:
    Conv2d(const size_t filter_num,
           const pss &filter,
           const pss &stride = {1, 1},
           const pss &padding = {0, 0},
           const tsss input_size = {0, 0, 0})

        : B(filter_num),
          dB(filter_num),
          filter_num(filter_num),

          filter(filter),
          stride(stride),
          padding(padding)
    {
        if (std::get<0>(input_size) != 0 and std::get<1>(input_size) != 0 and std::get<2>(input_size) != 0)
        {
            set(input_size);
        }
    }

    void set(const tsss &input_size) override
    {
        X = std::get<0>(input_size);
        Y = std::get<1>(input_size);
        Z = std::get<2>(input_size);
        Conv = std::vector<Conv_cell>(filter_num, Conv_cell(X, {Y, Z}, filter, stride, padding));
    }

    tsss get() override { return {filter_num, Conv[0].get_y_h(), Conv[0].get_y_w()}; };

    vvvd forward(const vvvd &x) override
    {
        vvvd y(filter_num);

        for (size_t i = 0; i < filter_num; ++i)
        {
            y[i] = Conv[i].forward(x);
        }

        // バイアス足すだけ
        for (size_t i = 0; i < y.size(); ++i)
        {
            for (size_t j = 0; j < y[i].size(); ++j)
            {
                for (size_t k = 0; k < y[i][j].size(); ++k)
                {
                    y[i][j][k] += B[i];
                }
            }
        }
        return y;
    }

    vvvd backward(const vvvd &dy) override
    {
        vvvd dx(X, vvd(Y, vd(Z)));

        for (size_t i = 0; i < filter_num; ++i)
        {
            dx = add(dx, Conv[i].backward(dy[i]));
        }

        // バイアスに足すだけ
        for (size_t i = 0; i < dy.size(); ++i)
        {
            for (size_t j = 0; j < dy[i].size(); ++j)
            {
                for (size_t k = 0; k < dy[i][j].size(); ++k)
                {
                    dB[i] += dy[i][j][k];
                }
            }
        }
        return dx;
    }

    void update(const double lr) override
    {
        dB = mul(dB, -lr);
        B = add(B, dB);
        clean(dB);

        for (size_t i = 0; i < Conv.size(); ++i)
        {
            Conv[i].update(lr);
        }
    }

    void clear() override
    {
        clean(dB);

        for (size_t i = 0; i < Conv.size(); ++i)
        {
            Conv[i].clear();
        }
    }
};