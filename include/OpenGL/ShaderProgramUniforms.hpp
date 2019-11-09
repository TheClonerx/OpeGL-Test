#include <glm.hpp>

// FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const float& value) noexcept
{
    glProgramUniform1f(m_handle, location, value);
}

// DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const double& value) noexcept
{
    glProgramUniform1d(m_handle, location, value);
}

// UINT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const uint32_t& value) noexcept
{
    glProgramUniform1ui(m_handle, location, value);
}

// INT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const int32_t& value) noexcept
{
    glProgramUniform1i(m_handle, location, value);
}

// UINT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const uint64_t& value) noexcept
{
    glProgramUniform1ui64ARB(m_handle, location, value);
}

// INT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const int64_t& value) noexcept
{
    glProgramUniform1i64ARB(m_handle, location, value);
}

// VEC 2 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec2& value) noexcept
{
    glProgramUniform2fv(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 3 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec3& value) noexcept
{
    glProgramUniform3fv(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 4 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec4& value) noexcept
{
    glProgramUniform4fv(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 2 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<2, double>& value) noexcept
{
    glProgramUniform2dv(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 3 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<3, double>& value) noexcept
{
    glProgramUniform3dv(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 4 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<4, double>& value) noexcept
{
    glProgramUniform4dv(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 2 UINT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<2, uint32_t>& value) noexcept
{
    glProgramUniform2uiv(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 3 UINT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<3, uint32_t>& value) noexcept
{
    glProgramUniform3uiv(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 4 UINT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<4, uint32_t>& value) noexcept
{
    glProgramUniform4uiv(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 2 INT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<2, int32_t>& value) noexcept
{
    glProgramUniform2iv(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 3 INT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<3, int32_t>& value) noexcept
{
    glProgramUniform3iv(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 4 INT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<4, int32_t>& value) noexcept
{
    glProgramUniform4iv(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 2 UINT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<2, uint64_t>& value) noexcept
{
    glProgramUniform2ui64vARB(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 3 UINT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<3, uint64_t>& value) noexcept
{
    glProgramUniform3ui64vARB(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 4 UINT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<4, uint64_t>& value) noexcept
{
    glProgramUniform4ui64vARB(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 2 INT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<2, int64_t>& value) noexcept
{
    glProgramUniform2i64vARB(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 3 INT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<3, int64_t>& value) noexcept
{
    glProgramUniform3i64vARB(m_handle, location, 1, glm::value_ptr(value));
}

// VEC 4 INT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<4, int64_t>& value) noexcept
{
    glProgramUniform4i64vARB(m_handle, location, 1, glm::value_ptr(value));
}

// MAT 2x2 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<2, 2, float>& value) noexcept
{
    glProgramUniformMatrix2fv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 3x2 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<3, 2, float>& value) noexcept
{
    glProgramUniformMatrix3x2fv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 4x2 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<4, 2, float>& value) noexcept
{
    glProgramUniformMatrix4x2fv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 2x3 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<2, 3, float>& value) noexcept
{
    glProgramUniformMatrix2x3fv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 3x3 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<3, 3, float>& value) noexcept
{
    glProgramUniformMatrix3fv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 4x3 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<4, 3, float>& value) noexcept
{
    glProgramUniformMatrix4x3fv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 2x4 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<2, 4, float>& value) noexcept
{
    glProgramUniformMatrix2x4fv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 3x4 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<3, 4, float>& value) noexcept
{
    glProgramUniformMatrix3x4fv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 4x4 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<4, 4, float>& value) noexcept
{
    glProgramUniformMatrix4fv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 2x2 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<2, 2, double>& value) noexcept
{
    glProgramUniformMatrix2dv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 3x2 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<3, 2, double>& value) noexcept
{
    glProgramUniformMatrix3x2dv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 4x2 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<4, 2, double>& value) noexcept
{
    glProgramUniformMatrix4x2dv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 2x3 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<2, 3, double>& value) noexcept
{
    glProgramUniformMatrix2x3dv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 3x3 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<3, 3, double>& value) noexcept
{
    glProgramUniformMatrix3dv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 4x3 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<4, 3, double>& value) noexcept
{
    glProgramUniformMatrix4x3dv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 2x4 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<2, 4, double>& value) noexcept
{
    glProgramUniformMatrix2x4dv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 3x4 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<3, 4, double>& value) noexcept
{
    glProgramUniformMatrix3x4dv(m_handle, location, 1, false, glm::value_ptr(value));
}

// MAT 4x4 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<4, 4, double>& value) noexcept
{
    glProgramUniformMatrix4dv(m_handle, location, 1, false, glm::value_ptr(value));
}
