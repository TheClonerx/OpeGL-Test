#pragma once
#include "Object.hpp"

namespace OpenGL {

class VertexArray : public Object {
public:
    inline VertexArray() noexcept
        : Object {}
    {
    }

    ~VertexArray();

    void create() noexcept final;

    void bind() noexcept;
};

} // namespace OpenGL
