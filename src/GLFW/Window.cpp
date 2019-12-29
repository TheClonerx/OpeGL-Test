#include <OpenGL/GL.hpp>
#include <GLFW/Window.hpp>
#include <GLFW/glfw3.h>
#include <Image.hpp>
#include <utility>
#include <string>

GLFW::Window::Window(Window&& rhs) noexcept
    : m_impl_window { std::exchange(rhs.m_impl_window, nullptr) }
{
}

GLFW::Window& GLFW::Window::operator=(Window&& rhs) noexcept
{
    m_impl_window = std::exchange(rhs.m_impl_window, nullptr);
    return *this;
}

GLFW::Window::~Window()
{
    destroy();
}

void GLFW::Window::setIcon(const tcx::Image& image)
{
    // glfwSetWindowIcon won't change the data so const_cast is safe
    GLFWimage glfw_image { image.size().x, image.size().y, const_cast<uint8_t*>(image.data()) };
    glfwSetWindowIcon(m_impl_window, 1, &glfw_image);
}

bool GLFW::Window::is_open()
{
    return m_impl_window;
}

void GLFW::Window::update()
{
    if (!is_open())
        return;
    before_update();

    glfwPollEvents();
    if (glfwWindowShouldClose(m_impl_window))
        destroy();
    on_update();
}

void GLFW::Window::makeContextCurrent()
{
    assert(is_open() && "The window isn't open");
    glfwMakeContextCurrent(m_impl_window);
}

void GLFW::Window::destroy()
{
    if (m_impl_window) {
        glfwDestroyWindow(m_impl_window);
        m_impl_window = nullptr;
    }
}

glm::ivec2 GLFW::Window::size() const noexcept
{
    glm::ivec2 ret;
    glfwGetWindowSize(m_impl_window, &ret.x, &ret.y);
    return ret;
}

void GLFW::Window::display()
{
    if (!is_open())
        return;
    before_display();
    glfwSwapBuffers(m_impl_window);
    on_display();
}

void GLFW::Window::create(glm::ivec2 size, std::string_view title)
{
    assert(!is_open() && "Window is already open");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    m_impl_window = glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_impl_window, this);
    setup_callbacks();
    auto current_context = glfwGetCurrentContext();
    makeContextCurrent();
    if (int error = glewInit(); error != GLEW_OK)
        throw std::runtime_error("ERROR:GLEW:" + std::to_string(error) + ":" + reinterpret_cast<const char*>(glewGetErrorString(error)));
    on_create();
}
