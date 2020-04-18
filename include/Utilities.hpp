#pragma once

#include <algorithm>
#include <chrono>
#include <glm/glm.hpp>
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
class TimeIt {
public:
    TimeIt(std::string_view name)
        : m_start { clock_t::now() }
        , m_name { name }
    {
    }

    ~TimeIt()
    {
        auto stop = clock_t::now();
        auto delta = std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(stop - m_start);
        std::fwprintf(stderr, L"%.*s took %.3fμs\n", static_cast<int>(m_name.size()), m_name.data(), delta);
    }

private:
    using clock_t = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock, std::chrono::steady_clock>;
    clock_t::time_point m_start;
    std::string_view m_name;
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