#pragma once

#include <iostream>
#include <vector>

// 入力
inline void in() {}

template <typename T>
inline void in(T &t) { std::cin >> t; }

template <typename T>
inline void in(std::vector<T> &t)
{
    for (size_t i = 0; i < t.size(); i++)
    {
        in(t[i]);
    }
}

template <typename T, typename... U>
inline void in(T &t, U &&...u)
{
    in(t);
    in(std::forward<U>(u)...);
}