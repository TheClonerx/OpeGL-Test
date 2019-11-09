#pragma once
#include "Object.hpp"
#include <GL/glew.h>
#include <string>
#include <string_view>

namespace OpenGL {

class ShaderProgram;

namespace impl {
    class BaseShader : public Object {
    protected:
        using Object::Object;

    public:
        void source(std::string_view) noexcept;
        std::string source() const;

        int status() const noexcept;
        std::string info_log() const;

        bool compile() noexcept;

        ~BaseShader();
    };
}

template <int Type>
class Shader : public impl::BaseShader {
public:
    constexpr Shader() noexcept
        : BaseShader {}
    {
    }

    void create() noexcept final;

    friend ShaderProgram;

    ~Shader() = default;
};

using FragmentShader = Shader<GL_FRAGMENT_SHADER>;
using VertexShader = Shader<GL_VERTEX_SHADER>;
using ComputeShader = Shader<GL_COMPUTE_SHADER>;
using GeometryShader = Shader<GL_GEOMETRY_SHADER>;
using TessControlShader = Shader<GL_TESS_CONTROL_SHADER>;

} // namespace OpenGL
