#pragma once
#include <OpenGL/GL.hpp>
#include <OpenGL/Object.hpp>
#include <iterator>
#include <type_traits>
#include <vector>

namespace OpenGL {

template <int Target>
class Buffer : public Object {
public:
    inline constexpr Buffer() noexcept
        : Object {}
    {
    }

    ~Buffer()
    {
        if (m_handle != invalid_handle) {
            glDeleteBuffers(1, &m_handle);
            m_handle = invalid_handle;
        }
    }

    inline void create() noexcept final
    {
        glGenBuffers(1, &m_handle);
    }

    inline void bind() noexcept
    {
        glBindBuffer(Target, m_handle);
    }

    template <typename It>
    auto data(It start, It stop, int usage) noexcept -> std::enable_if_t<std::is_trivially_copyable_v<typename std::iterator_traits<It>::value_type>>
    {
        if constexpr (std::is_pointer_v<It>) {
            glNamedBufferData(m_handle, reinterpret_cast<const char*>(stop) - reinterpret_cast<const char*>(start), start, usage);
        } else {
            std::vector<typename std::iterator_traits<It>::value_type> dat(start, stop);
            data(dat.data(), dat.data() + dat.size(), usage);
        }
    }

    template <typename T>
    auto data(T&& dat, int usage) noexcept
    {
        return data(std::begin(dat), std::end(dat), usage);
    }
};

} // namespace OpenGL
