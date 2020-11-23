#include <OpenGL/BindGuard.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const float& value) noexcept
{
#ifdef glProgramUniform1f
    glProgramUniform1f(m_handle, location, value);
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform1f(location, value);
#endif
}

#ifdef GL_ARB_gpu_shader_fp64

// DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const double& value) noexcept
{
#ifdef glProgramUniform1d
    glProgramUniform1d(m_handle, location, value);
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform1d(location, value);
#endif
}

#endif

// UINT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const uint32_t& value) noexcept
{
#ifdef glProgramUniform1ui
    glProgramUniform1ui(m_handle, location, value);
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform1ui(location, value);
#endif
}

// INT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const int32_t& value) noexcept
{
#ifdef glProgramUniform1i
    glProgramUniform1i(m_handle, location, value);
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform1i(location, value);
#endif
}

#ifdef GL_ARB_gpu_shader_int64

// UINT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const uint64_t& value) noexcept
{
#ifdef glProgramUniform1ui64ARB
    glProgramUniform1ui64ARB(m_handle, location, value);
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform1ui64ARB(location, value);
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_int64

// INT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const int64_t& value) noexcept
{
#ifdef glProgramUniform1i64ARB
    glProgramUniform1i64ARB(m_handle, location, value);
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform1i64ARB(location, value);
#endif
}

#endif

// VEC 2 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec2& value) noexcept
{
#ifdef glProgramUniform2fv
    glProgramUniform2fv(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform2fv(location, 1, glm::value_ptr(value));
#endif
}

// VEC 3 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec3& value) noexcept
{
#ifdef glProgramUniform3fv
    glProgramUniform3fv(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform3fv(location, 1, glm::value_ptr(value));
#endif
}

// VEC 4 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec4& value) noexcept
{
#ifdef glProgramUniform4fv
    glProgramUniform4fv(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform4fv(location, 1, glm::value_ptr(value));
#endif
}

#ifdef GL_ARB_gpu_shader_fp64

// VEC 2 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<2, double>& value) noexcept
{
#ifdef glProgramUniform2dv
    glProgramUniform2dv(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform2dv(location, 1, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_fp64

// VEC 3 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<3, double>& value) noexcept
{
#ifdef glProgramUniform3dv
    glProgramUniform3dv(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform3dv(location, 1, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_fp64

// VEC 4 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<4, double>& value) noexcept
{
#ifdef glProgramUniform4dv
    glProgramUniform4dv(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform4dv(location, 1, glm::value_ptr(value));
#endif
}

#endif

// VEC 2 UINT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<2, uint32_t>& value) noexcept
{
#ifdef glProgramUniform2uiv
    glProgramUniform2uiv(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform2uiv(location, 1, glm::value_ptr(value));
#endif
}

// VEC 3 UINT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<3, uint32_t>& value) noexcept
{
#ifdef glProgramUniform3uiv
    glProgramUniform3uiv(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform3uiv(location, 1, glm::value_ptr(value));
#endif
}

// VEC 4 UINT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<4, uint32_t>& value) noexcept
{
#ifdef glProgramUniform4uiv
    glProgramUniform4uiv(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform4uiv(location, 1, glm::value_ptr(value));
#endif
}

// VEC 2 INT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<2, int32_t>& value) noexcept
{
#ifdef glProgramUniform2iv
    glProgramUniform2iv(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform2iv(location, 1, glm::value_ptr(value));
#endif
}

// VEC 3 INT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<3, int32_t>& value) noexcept
{
#ifdef glProgramUniform3iv
    glProgramUniform3iv(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform3iv(location, 1, glm::value_ptr(value));
#endif
}

// VEC 4 INT32_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<4, int32_t>& value) noexcept
{
#ifdef glProgramUniform4iv
    glProgramUniform4iv(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform4iv(location, 1, glm::value_ptr(value));
#endif
}

#ifdef GL_ARB_gpu_shader_int64

// VEC 2 UINT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<2, uint64_t>& value) noexcept
{
#ifdef glProgramUniform2ui64vARB
    glProgramUniform2ui64vARB(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform2ui64vARB(location, 1, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_int64

// VEC 3 UINT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<3, uint64_t>& value) noexcept
{
#ifdef glProgramUniform3ui64vARB
    glProgramUniform3ui64vARB(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform3ui64vARB(location, 1, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_int64

// VEC 4 UINT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<4, uint64_t>& value) noexcept
{
#ifdef glProgramUniform4ui64vARB
    glProgramUniform4ui64vARB(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform4ui64vARB(location, 1, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_int64

// VEC 2 INT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<2, int64_t>& value) noexcept
{
#ifdef glProgramUniform2i64vARB
    glProgramUniform2i64vARB(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform2i64vARB(location, 1, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_int64

// VEC 3 INT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<3, int64_t>& value) noexcept
{
#ifdef glProgramUniform3i64vARB
    glProgramUniform3i64vARB(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform3i64vARB(location, 1, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_int64

// VEC 4 INT64_T
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::vec<4, int64_t>& value) noexcept
{
#ifdef glProgramUniform4i64vARB
    glProgramUniform4i64vARB(m_handle, location, 1, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniform4i64vARB(location, 1, glm::value_ptr(value));
#endif
}

#endif

// MAT 2x2 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<2, 2, float>& value) noexcept
{
#ifdef glProgramUniformMatrix2fv
    glProgramUniformMatrix2fv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix2fv(location, 1, false, glm::value_ptr(value));
#endif
}

// MAT 3x2 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<3, 2, float>& value) noexcept
{
#ifdef glProgramUniformMatrix3x2fv
    glProgramUniformMatrix3x2fv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix3x2fv(location, 1, false, glm::value_ptr(value));
#endif
}

// MAT 4x2 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<4, 2, float>& value) noexcept
{
#ifdef glProgramUniformMatrix4x2fv
    glProgramUniformMatrix4x2fv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix4x2fv(location, 1, false, glm::value_ptr(value));
#endif
}

// MAT 2x3 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<2, 3, float>& value) noexcept
{
#ifdef glProgramUniformMatrix2x3fv
    glProgramUniformMatrix2x3fv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix2x3fv(location, 1, false, glm::value_ptr(value));
#endif
}

// MAT 3x3 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<3, 3, float>& value) noexcept
{
#ifdef glProgramUniformMatrix3fv
    glProgramUniformMatrix3fv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix3fv(location, 1, false, glm::value_ptr(value));
#endif
}

// MAT 4x3 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<4, 3, float>& value) noexcept
{
#ifdef glProgramUniformMatrix4x3fv
    glProgramUniformMatrix4x3fv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix4x3fv(location, 1, false, glm::value_ptr(value));
#endif
}

// MAT 2x4 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<2, 4, float>& value) noexcept
{
#ifdef glProgramUniformMatrix2x4fv
    glProgramUniformMatrix2x4fv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix2x4fv(location, 1, false, glm::value_ptr(value));
#endif
}

// MAT 3x4 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<3, 4, float>& value) noexcept
{
#ifdef glProgramUniformMatrix3x4fv
    glProgramUniformMatrix3x4fv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix3x4fv(location, 1, false, glm::value_ptr(value));
#endif
}

// MAT 4x4 FLOAT
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<4, 4, float>& value) noexcept
{
#ifdef glProgramUniformMatrix4fv
    glProgramUniformMatrix4fv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
#endif
}

#ifdef GL_ARB_gpu_shader_fp64

// MAT 2x2 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<2, 2, double>& value) noexcept
{
#ifdef glProgramUniformMatrix2dv
    glProgramUniformMatrix2dv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix2dv(location, 1, false, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_fp64

// MAT 3x2 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<3, 2, double>& value) noexcept
{
#ifdef glProgramUniformMatrix3x2dv
    glProgramUniformMatrix3x2dv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix3x2dv(location, 1, false, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_fp64

// MAT 4x2 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<4, 2, double>& value) noexcept
{
#ifdef glProgramUniformMatrix4x2dv
    glProgramUniformMatrix4x2dv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix4x2dv(location, 1, false, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_fp64

// MAT 2x3 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<2, 3, double>& value) noexcept
{
#ifdef glProgramUniformMatrix2x3dv
    glProgramUniformMatrix2x3dv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix2x3dv(location, 1, false, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_fp64

// MAT 3x3 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<3, 3, double>& value) noexcept
{
#ifdef glProgramUniformMatrix3dv
    glProgramUniformMatrix3dv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix3dv(location, 1, false, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_fp64

// MAT 4x3 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<4, 3, double>& value) noexcept
{
#ifdef glProgramUniformMatrix4x3dv
    glProgramUniformMatrix4x3dv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix4x3dv(location, 1, false, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_fp64

// MAT 2x4 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<2, 4, double>& value) noexcept
{
#ifdef glProgramUniformMatrix2x4dv
    glProgramUniformMatrix2x4dv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix2x4dv(location, 1, false, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_fp64

// MAT 3x4 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<3, 4, double>& value) noexcept
{
#ifdef glProgramUniformMatrix3x4dv
    glProgramUniformMatrix3x4dv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix3x4dv(location, 1, false, glm::value_ptr(value));
#endif
}

#endif

#ifdef GL_ARB_gpu_shader_fp64

// MAT 4x4 DOUBLE
template <>
inline void OpenGL::ShaderProgram::uniform(int location, const glm::mat<4, 4, double>& value) noexcept
{
#ifdef glProgramUniformMatrix4dv
    glProgramUniformMatrix4dv(m_handle, location, 1, false, glm::value_ptr(value));
#else
    GL::BindGuard<GL_CURRENT_PROGRAM> guard { m_handle };
    glUniformMatrix4dv(location, 1, false, glm::value_ptr(value));
#endif
}

#endif
