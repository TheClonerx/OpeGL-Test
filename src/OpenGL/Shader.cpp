#include "OpenGL/Shader.hpp"

OpenGL::impl::BaseShader::~BaseShader()
{
    if (m_handle != invalid_handle) {
        glDeleteShader(m_handle);
        m_handle = invalid_handle;
    }
}

void OpenGL::impl::BaseShader::source(std::string_view src) noexcept
{
    const char* const ptr = src.data();
    int size = src.size();
    glShaderSource(m_handle, 1, &ptr, &size);
}

std::string OpenGL::impl::BaseShader::source() const
{
    int size;
    glGetShaderiv(m_handle, GL_SHADER_SOURCE_LENGTH, &size);
    std::string src(size, '\0');
    glGetShaderSource(m_handle, size, &size, src.data());
    return src;
}

int OpenGL::impl::BaseShader::status() const noexcept
{
    int stat;
    glGetShaderiv(m_handle, GL_COMPILE_STATUS, &stat);
    return stat;
}

std::string OpenGL::impl::BaseShader::info_log() const
{
    int size;
    glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &size);
    std::string log(size, '\0');
    glGetShaderInfoLog(m_handle, size, &size, log.data());
    return log;
}

bool OpenGL::impl::BaseShader::compile() noexcept
{
    glCompileShader(m_handle);
    return status();
}

template <>
void OpenGL::FragmentShader::create() noexcept
{
    m_handle = glCreateShader(GL_FRAGMENT_SHADER);
}

template <>
void OpenGL::VertexShader::create() noexcept
{
    m_handle = glCreateShader(GL_VERTEX_SHADER);
}

template <>
void OpenGL::ComputeShader::create() noexcept
{
    m_handle = glCreateShader(GL_COMPUTE_SHADER);
}

template <>
void OpenGL::GeometryShader::create() noexcept
{
    m_handle = glCreateShader(GL_GEOMETRY_SHADER);
}

template <>
void OpenGL::TessControlShader::create() noexcept
{
    m_handle = glCreateShader(GL_TESS_CONTROL_SHADER);
}