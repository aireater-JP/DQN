#pragma once

#include <fstream>

class fin
{
    std::ifstream s;

public:
    fin(std::string name) : s(name){};

    void operator()(){};

    template <typename T>
    void operator()(T &x)
    {
        s >> x;
    }

    template <typename T>
    void operator()(std::vector<T> &x)
    {
        size_t size;
        operator()(size);
        x = std::vector<T>(size);
        for (size_t i = 0; i < size; ++i)
        {
            operator()(x[i]);
        }
    }

    void operator()(std::pair<std::size_t, std::size_t> &x)
    {
        operator()(x.first, x.second);
    }

    void operator()(std::tuple<std::size_t, std::size_t, std::size_t> &x)
    {
        size_t a, b, c;
        operator()(a, b, c);
        x = {a, b, c};
    }

    template <typename T, typename... U>
    void operator()(T &t, U &&...u)
    {
        operator()(t);
        operator()(std::forward<U>(u)...);
    }
};

class fout
{
    std::ofstream s;

public:
    fout(std::string name) : s(name){};

    void operator()() {}

    template <typename T>
    void operator()(const T &x)
    {
        s << x << " ";
    }

    template <typename T>
    void operator()(const std::vector<T> &x)
    {
        s << x.size() << "\n";
        for (size_t i = 0; i < x.size(); ++i)
        {
            operator()(x[i]);
        }
        newline();
    }

    void operator()(const std::pair<std::size_t, std::size_t> &x)
    {
        s << x.first << " " << x.second << " ";
        newline();
    }

    void operator()(const std::tuple<std::size_t, std::size_t, std::size_t> &x)
    {
        s << std::get<0>(x) << " " << std::get<1>(x) << " " << std::get<2>(x) << " ";
        newline();
    }

    template <typename T, typename... U>
    void operator()(const T &t, U &&...u)
    {
        operator()(t);
        operator()(std::forward<U>(u)...);
    }

    void newline()
    {
        s << "\n";
    }
};