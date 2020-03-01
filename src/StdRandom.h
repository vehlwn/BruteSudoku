#pragma once

#include <random>
#include <type_traits>

namespace vehlwn {
class StdRandom
{
public:
    static thread_local std::mt19937 gen;

    template<typename T, typename U>
    static auto next(T a, U b)
        -> std::enable_if_t<std::is_integral_v<T> && std::is_integral_v<U>,
            std::common_type_t<T, U>>
    {
        using ret_t = std::common_type_t<T, U>;
        std::uniform_int_distribution<ret_t> dist{
            static_cast<ret_t>(a), static_cast<ret_t>(b)};
        return dist(gen);
    }
};
} // namespace vehlwn
