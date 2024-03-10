#pragma once

#include <vector>
#include <tuple>

using vd = std::vector<double>;
using vvd = std::vector<vd>;
using vvvd = std::vector<vvd>;

using size_t = std::size_t;

using pss = std::pair<size_t, size_t>;

using tsss = std::tuple<size_t, size_t, size_t>;

double add(const double a, const double b)
{
    return a + b;
}

template <typename T>
std::vector<T> add(const std::vector<T> &a, const std::vector<T> &b)
{
    std::vector<T> c(a.size());
    for (size_t i = 0; i < a.size(); ++i)
    {
        c[i] = add(a[i], b[i]);
    }
    return c;
}

double sub(const double a, const double b)
{
    return a - b;
}

template <typename T>
std::vector<T> sub(const std::vector<T> &a, const std::vector<T> &b)
{
    std::vector<T> c(a.size());
    for (size_t i = 0; i < a.size(); ++i)
    {
        c[i] = sub(a[i], b[i]);
    }
    return c;
}

double mul(const double a, const double b)
{
    return a * b;
}

template <typename T>
std::vector<T> mul(const std::vector<T> &a, const std::vector<T> &b)
{
    std::vector<T> c(a.size());
    for (size_t i = 0; i < a.size(); ++i)
    {
        c[i] = mul(a[i], b[i]);
    }
    return c;
}

template <typename T>
std::vector<T> mul(const std::vector<T> &a, const double &b)
{
    std::vector<T> c(a.size());
    for (size_t i = 0; i < a.size(); ++i)
    {
        c[i] = mul(a[i], b);
    }
    return c;
}

template <typename T>
void clean(std::vector<T> &x)
{
    std::fill(x.begin(), x.end(), 0);
}

template <typename T>
void clean(std::vector<std::vector<T>> &x)
{
    for (size_t i = 0; i < x.size(); ++i)
    {
        clean(x[i]);
    }
}