#include "OpenGL/Object.hpp"

OpenGL::Object::~Object()
{
    // make sure the derived class properly destroyed the object
    // VRAM tends to be way less than RAM, so we must be more strict
    assert(
        m_handle == invalid_handle
        && "You forgot to call the destructor for a OpenGL object");
}