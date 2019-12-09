#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Image.hpp>
#include <Window.hpp>
#include <utility>

tcx::Window::Window(Window&& rhs) noexcept
    : m_impl_window { std::exchange(rhs.m_impl_window, nullptr) }
{
}

tcx::Window& tcx::Window::operator=(Window&& rhs) noexcept
{
    m_impl_window = std::exchange(rhs.m_impl_window, nullptr);
    return *this;
}

tcx::Window::~Window()
{
    destroy();
}

void tcx::Window::setIcon(const Image& image)
{
    // glfwSetWindowIcon won't change the data so const_cast is safe
    GLFWimage glfw_image { image.size().x, image.size().y, const_cast<uint8_t*>(image.data()) };
    glfwSetWindowIcon(m_impl_window, 1, &glfw_image);
}

bool tcx::Window::is_open()
{
    return m_impl_window;
}

void tcx::Window::update()
{
    assert(is_open() && "The window isn't open");
    glfwPollEvents();
    if (glfwWindowShouldClose(m_impl_window))
        destroy();
}

void tcx::Window::makeContextCurrent()
{
    assert(is_open() && "The window isn't open");
    glfwMakeContextCurrent(m_impl_window);
}

void tcx::Window::destroy()
{
    if (m_impl_window) {
        glfwDestroyWindow(m_impl_window);
        m_impl_window = nullptr;
    }
}

glm::ivec2 tcx::Window::size() const noexcept
{
    glm::ivec2 ret;
    glfwGetWindowSize(m_impl_window, &ret.x, &ret.y);
    return ret;
}

void tcx::Window::display()
{
    assert(is_open() && "The window isn't open");
    glfwSwapBuffers(m_impl_window);
}

void tcx::Window::create(glm::ivec2 size, std::string_view title)
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
}