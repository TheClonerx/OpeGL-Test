#include "OpenGL/ShaderProgram.hpp"

void OpenGL::ShaderProgram::create() noexcept
{
    m_handle = glCreateProgram();
}

void OpenGL::ShaderProgram::bind() noexcept
{
    glUseProgram(m_handle);
}

void OpenGL::ShaderProgram::unbind() noexcept
{
    glUseProgram(0);
}

OpenGL::ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_handle);
    m_handle = invalid_handle;
}

bool OpenGL::ShaderProgram::link() noexcept
{
    glLinkProgram(m_handle);
    return status();
}

int OpenGL::ShaderProgram::status() const noexcept
{
    int stat;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &stat);
    return stat;
}

std::string OpenGL::ShaderProgram::info_log() const noexcept
{
    int size;
    glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &size);
    std::string log(size, '\0');
    glGetProgramInfoLog(m_handle, size, &size, log.data());
    return log;
}
