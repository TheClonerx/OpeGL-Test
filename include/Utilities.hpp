#pragma once

#include <chrono>
#include <string_view>

namespace tcx { // here i place my stuff

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

}