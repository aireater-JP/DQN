#include <iostream>
#include <vector>

inline void newline()
{
    std::cout << std::endl;
}

// 出力
inline void out(){};

template <typename T>
inline void out(const T &x) { std::cout << x << " "; }

template <typename T>
inline void out(const std::vector<T> &t)
{
    out("[");
    for (size_t i = 0; i < t.size(); i++)
    {
        out(t[i]);
    }
    out("]");
    newline();
}

template <typename T, typename... U>
inline void out(const T &t, U &&...u)
{
    out(t);
    out(std::forward<U>(u)...);
}