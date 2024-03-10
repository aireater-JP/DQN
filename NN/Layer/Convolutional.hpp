#pragma once

#include "../Random.hpp"

size_t output_size(size_t x, size_t f, size_t p, size_t s)
{
    if (p == -1)
    {
        return x;
    }
    return (x - f + 2 * p) / s + 1;
}

class Convolutional
{
    vvd F;
    vvd dF;

    size_t stride_h, stride_w;
    size_t padding_h, padding_w;

    vvd input;

    size_t x_h, x_w;
    size_t f_h, f_w;

    size_t y_h, y_w;

public:
    /*
    入力サイズ
    フィルターサイズ
    ストライドの幅
    パディング
    padding=-1 入力サイズと出力サイズが等しくなる
    */
    Convolutional(const pss &input_size,
                  const pss &filter,
                  const pss &stride = {1, 1},
                  const pss &padding = {0, 0})

        : F(filter.first, vd(filter.second)),
          dF(filter.first, vd(filter.second)),

          x_h(input_size.first), x_w(input_size.second),
          f_h(filter.first), f_w(filter.second),

          stride_h(stride.first), stride_w(stride.second),
          padding_h(padding.first), padding_w(padding.second),

          y_h(output_size(x_h, f_h, padding_h, stride_h)),
          y_w(output_size(x_w, f_w, padding_w, stride_w))
    {
        // 例外
        if (padding_h == -1 or padding_w == -1)
        {
            padding_h = (stride_h * x_h - stride_h - x_h + f_h) / 2;
            padding_w = (stride_w * x_w - stride_w - x_w + f_w) / 2;
        }

        Random<std::normal_distribution<>> r(0.0, 1.0);

        for (size_t i = 0; i < f_h; ++i)
        {
            for (size_t j = 0; j < f_w; ++j)
            {
                F[i][j] = r();
            }
        }
    }

    size_t get_y_h() { return y_h; }
    size_t get_y_w() { return y_w; }

    vvd forward(const vvd &x)
    {
        input = x;

        vvd y(y_h, vd(y_w));

        for (size_t i = 0; i < y_h; ++i)
        {
            for (size_t j = 0; j < y_w; ++j)
            {
                for (size_t m = 0; m < f_h; ++m)
                {
                    for (size_t n = 0; n < f_w; ++n)
                    {
                        size_t row = i * stride_h + m - padding_h;
                        size_t col = j * stride_w + n - padding_w;
                        if (row < x_h && col < x_w)
                        {
                            y[i][j] += x[row][col] * F[m][n];
                        }
                    }
                }
            }
        }
        return y;
    }

    vvd backward(const vvd &dy)
    {
        vvd dx(x_h, vd(x_w));

        for (size_t i = 0; i < y_h; ++i)
        {
            for (size_t j = 0; j < y_w; ++j)
            {
                for (size_t m = 0; m < f_h; ++m)
                {
                    for (size_t n = 0; n < f_w; ++n)
                    {
                        size_t row = i * stride_h + m - padding_h;
                        size_t col = j * stride_w + n - padding_w;
                        if (row < x_h && col < x_w)
                        {
                            dx[row][col] += dy[i][j] * F[m][n];
                            dF[m][n] += dy[i][j] * input[row][col];
                        }
                    }
                }
            }
        }
        return dx;
    }

    void update(const double lr)
    {
        dF = mul(dF, -lr);
        F = add(F, dF);
        clean(dF);
    }

    void clear()
    {
        clean(dF);
    }
};