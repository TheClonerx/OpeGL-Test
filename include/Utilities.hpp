#pragma once

#include <algorithm>
#include <chrono>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <string_view>
#include <vector>

namespace tcx { // here i place my stuff

template <typename T>
struct VectorHasher {
};

template <glm::length_t I, typename T, glm::qualifier Q>
struct VectorHasher<glm::vec<I, T, Q>> {
    static_assert(std::is_trivially_copyable_v<T>);
    using result_type = std::size_t;
    using argument_type = glm::vec<I, T, Q>;
    result_type operator()(argument_type const& v) const noexcept
    {
        return std::hash<std::string_view> {}({ reinterpret_cast<char const*>(&v), sizeof(v) });
    }
};

template <typename R, typename T, typename F, std::size_t... I, typename... Args>
R for_each_index(T&& o, F&& f, std::index_sequence<I...>, Args&&... args)
{
    return R { f(o[I], std::forward<Args>(args)...)... };
}

template <typename T0, typename T1>
auto positive_modulus(T0 x, T1 y) noexcept //similar to x % y in python
{
    static_assert((std::is_integral_v<T0> && std::is_integral_v<T1>) || (std::is_floating_point_v<T0> && std::is_floating_point_v<T1>), "Either both types are integer, or both types are floating point");

    if constexpr (std::is_integral_v<T0>) {
        return static_cast<std::make_unsigned_t<T0>>(x) % y;
    } else {
        auto result = std::fmod(x, y);
        if (result && x < 0)
            return -result;
        else
            return result;
    }
}

template <glm::length_t I, typename T0, typename T1, glm::qualifier Q>
auto positive_modulus(glm::vec<I, T0, Q> const& x, T1 y) noexcept //calls positive_modulus(x[i], y) for each value on the vector
{
    static_assert((std::is_integral_v<T0> && std::is_integral_v<T1>) || (std::is_floating_point_v<T0> && std::is_floating_point_v<T1>), "Either both types are integer, or both types are floating point");

    auto f = positive_modulus<T0, T1>;
    if constexpr (std::is_integral_v<T0>) {
        return for_each_index<glm::vec<I, std::make_unsigned_t<T0>, Q>>(x, f, std::make_index_sequence<I> {}, y);
    } else {
        return for_each_index<glm::vec<I, T0, Q>>(x, f, std::make_index_sequence<I> {}, y);
    }
}

template <typename T0, typename T1>
auto floor_division(T0 x, T1 y) noexcept //similar to x//y in python
{
    using result_type = decltype(x / y);
    if constexpr (std::is_integral_v<result_type>) {
        if ((x < 0 && y < 0) || (x > 0 && y > 0)) { // same sign, do nothing
            return x / y;
        } else {
            return x / y - 1;
        }
    } else {
        return std::floor(x / y);
    }
}

template <glm::length_t I, typename T0, typename T1, glm::qualifier Q>
auto floor_division(glm::vec<I, T0, Q> const& x, T1 y) noexcept //calls floor_division(x[i], y) for each value on the vector
{
    using result_type = decltype(std::declval<T0>() / y);
    auto f = floor_division<T0, T1>;
    return for_each_index<glm::vec<I, result_type, Q>>(x, f, std::make_index_sequence<I> {}, y);
}

class TimeIt {
public:
    template <typename... Args>
    TimeIt(std::string_view format, Args&&... args)
        : m_start { clock_t::now() }
        , m_name { fmt::format(format, std::forward<Args>(args)...) }
    {
    }

    ~TimeIt()
    {
        auto stop = clock_t::now();
        auto delta = std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(stop - m_start);
        spdlog::info("{} took {}", m_name, delta.count());
    }

private:
    using clock_t = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock, std::chrono::steady_clock>;
    clock_t::time_point m_start;
    std::string m_name;
};

inline std::string_view lstrip(std::string_view s)
{
    while (!s.empty() && std::isspace(s.front()))
        s.remove_prefix(1);
    return s;
}

inline std::string_view rstrip(std::string_view s)
{
    while (!s.empty() && std::isspace(s.back()))
        s.remove_suffix(1);
    return s;
}

inline std::string_view strip(std::string_view s)
{
    return lstrip(rstrip(s));
}

inline std::string_view lstrip(std::string_view s, std::string_view chars)
{
    while (!s.empty() && std::find(chars.cbegin(), chars.cend(), s.front()) != chars.cend())
        s.remove_prefix(1);
    return s;
}

inline std::string_view rstrip(std::string_view s, std::string_view chars)
{
    while (!s.empty() && std::find(chars.crbegin(), chars.crend(), s.front()) != chars.crend())
        s.remove_prefix(1);
    return s;
}

inline std::string_view strip(std::string_view s, std::string_view chars)
{
    return lstrip(rstrip(s, chars), chars);
}

inline bool startswith(std::string_view hay, std::string_view needle)
{
    return hay.size() >= needle.size() && hay.substr(0, needle.size()) == needle;
}

inline bool endswith(std::string_view hay, std::string_view needle)
{
    return hay.size() >= needle.size() && hay.substr(hay.size() - needle.size()) == needle;
}

std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> parse_ini(std::string_view name);
std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> parse_ini(std::istream& is);
}