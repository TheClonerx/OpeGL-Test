#include <GL/glew.h>
#include <OpenGL/VertexArray.hpp>
#include <array>

void OpenGL::VertexArray::create() noexcept
{
    glGenVertexArrays(1, &m_handle);
}

OpenGL::VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_handle);
    m_handle = invalid_handle;
}

void OpenGL::VertexArray::bind() noexcept
{
    glBindVertexArray(m_handle);
}