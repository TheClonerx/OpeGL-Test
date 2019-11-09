#pragma once
#include <cassert>
#include <cstdint>

namespace OpenGL {

class Object {
public:
    constexpr static uint32_t invalid_handle = ~0u;
    Object(Object&&) = delete;
    Object& operator=(Object&&) = delete;

    inline constexpr Object() noexcept
        : m_handle { invalid_handle }
    {
    }

protected:
    inline constexpr Object(uint32_t handle) noexcept
        : m_handle { handle }
    {
    }

public:
    inline uint32_t handle() noexcept
    {
        return m_handle;
    }

    virtual void create() noexcept = 0;

    virtual ~Object();

protected:
    uint32_t m_handle;
};

} // namespace OpenGL
