#pragma once
#include "Object.hpp"
#include "Shader.hpp"

namespace OpenGL {

class ShaderProgram : public Object {
public:
    constexpr inline ShaderProgram() noexcept : Object{}{}

    void create() noexcept final;

    void bind() noexcept;

    void unbind() noexcept;

    template <typename... Args>
    void attach(const Args&... args)
    {
        (glAttachShader(m_handle, args.m_handle), ...);
    }

    bool link() noexcept;
    int status() const noexcept;
    std::string info_log() const noexcept;

    template <typename T>
    void uniform(std::string_view name, const T& value) noexcept
    {
        uniform(name.data(), value);
    }

    template <typename T>
    void uniform(const char* name, const T& value) noexcept
    {
        int location = glGetUniformLocation(m_handle, name);
        uniform(location, value);
    }

    template <typename T>
    void uniform(int location, const T& name) noexcept; // does the thing;

    ~ShaderProgram();
};

} // namespace OpenGL

#include "ShaderProgramUniforms.hpp"