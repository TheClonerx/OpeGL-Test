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

        template <typename It>
        auto source(It start, It stop) noexcept -> std::enable_if_t<std::is_convertible_v<decltype(*start), std::string_view>>
        {
            std::vector<const char*> chunks;
            std::vector<int> chunk_sizes;

            for (; start != stop; ++start) {
                std::string_view chunk = *start;
                chunks.push_back(chunk.data());
                chunk_sizes.push_back(chunk.size());
            }

            glShaderSource(m_handle, chunks.size(), chunks.data(), chunk_sizes.data());
        }

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
