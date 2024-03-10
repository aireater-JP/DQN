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
            s << x[i] << " ";
        }
    }

    void operator()(const std::tuple<std::size_t, std::size_t, std::size_t> &x)
    {
        s << std::get<0>(x) << " " << std::get<1>(x) << " " << std::get<2>(x) << " ";
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