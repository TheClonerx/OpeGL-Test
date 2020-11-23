#pragma once

#include <OpenGL/GL.hpp>
#include <cstdint>

namespace GL {

template <int Name>
class BindGuard;

template <>
class BindGuard<GL_CURRENT_PROGRAM> {
public:
    explicit BindGuard(std::uint32_t newValue) noexcept
    {
        // sanity check
        static_assert(sizeof(int) == sizeof(std::uint32_t) && alignof(int) == alignof(std::uint32_t));
        glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<int*>(&m_oldValue));
        glUseProgram(newValue);
    }

    ~BindGuard()
    {
        glUseProgram(m_oldValue);
    }

private:
    std::uint32_t m_oldValue;
};

} // namespace GL
