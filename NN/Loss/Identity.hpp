#pragma once

#include "../Loss.hpp"

#include <algorithm>
#include <cmath>
#include <cfloat>

class Identity_with_Loss : public Loss
{
    vd m_y;
    vd m_t;

public:
    double forward(const vvvd &x, const vd &t)
    {
        m_t = t;
        m_y = Identity(x[0][0]);
        return sum_of_squared_error(m_y, t);
    }

    vvvd backward()
    {
        vvvd dx(1, vvd(1, vd(m_y.size())));
        dx[0][0] = sub(m_y, m_t);
        return dx;
    }

private:
    vd Identity(const vd &x)
    {
        return x;
    }

    double sum_of_squared_error(const vd &x, const vd &t)
    {
        double y = 0;
        for (size_t i = 0; i < x.size(); ++i)
        {
            y += std::pow(x[i] - t[i], 2);
        }
        return y * 0.5;
    }
};