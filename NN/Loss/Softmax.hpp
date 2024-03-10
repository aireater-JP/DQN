#pragma once

#include "../Loss.hpp"

#include <algorithm>
#include <cmath>
#include <cfloat>

class Softmax_with_Loss : public Loss
{
    vd m_y;
    vd m_t;

public:
    double forward(const vvvd &x, const vd &t)
    {
        m_t = t;
        m_y = softmax(x[0][0]);
        return cross_entropy_error(m_y, t);
    }

    vvvd backward()
    {
        vvvd dx(1, vvd(1, vd(m_y.size())));
        dx[0][0] = sub(m_y, m_t);
        return dx;
    }

private:
    vd softmax(const vd &x)
    {
        vd y(x.size());
        double C = *std::max_element(x.begin(), x.end());

        // Cを引いたexpを求める
        double sum = 0;
        for (size_t i = 0; i < x.size(); ++i)
        {
            y[i] = std::exp(x[i] - C);
            sum += y[i];
        }

        sum = 1.0 / sum;

        y = mul(y, sum);
        return y;
    }

    double cross_entropy_error(const vd &x, const vd &t)
    {
        double y = 0;
        for (size_t i = 0; i < x.size(); ++i)
        {
            y += t[i] * std::log(x[i] + DBL_MIN);
        }
        return -y;
    }
};