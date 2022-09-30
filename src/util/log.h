#ifndef HINAPE_MY_LOG_H
#define HINAPE_MY_LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace HinaUtil{
class Log
{
public:
    template <typename T>
    static auto info(const T &msg) -> void;

    template <typename T>
    static auto info(const std::vector<T> &vec) -> void;

    static std::ofstream o_info;
};

template<typename A, typename B>
auto operator<<(std::ofstream &o, const std::pair<A, B> &p) -> std::ofstream &
{
    o << p.first << " " << p.second;
    return o;
}

template<typename T>
auto Log::info(const T &msg) -> void
{
#ifdef _DEBUG
    o_info << msg << std::endl;
#endif
}

template<typename T>
auto Log::info(const std::vector<T> &vec) -> void
{
#ifdef _DEBUG
    for (auto &e: vec)
        o_info << e << std::endl;
#endif
}
}

#endif //HINAPE_MY_LOG_H
