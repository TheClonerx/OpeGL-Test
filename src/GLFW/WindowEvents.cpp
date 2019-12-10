#include <GLFW/Window.hpp>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

static void keyCallback(GLFWwindow* window_impl, int key, int scancode, int action, int mods)
{
    auto window = reinterpret_cast<GLFW::Window*>(glfwGetWindowUserPointer(window_impl));
    switch (action) {
    case GLFW_PRESS:
        window->on_key_press(key, mods);
        break;
    case GLFW_RELEASE:
        window->on_key_release(key, mods);
        break;
    case GLFW_REPEAT:
        window->on_key_repeat(key, mods);
        break;
    }
}

static void charCallback(GLFWwindow* window_impl, uint32_t codepoint)
{
    auto window = reinterpret_cast<GLFW::Window*>(glfwGetWindowUserPointer(window_impl));
    window->on_char(static_cast<char32_t>(codepoint));
}

static void dropCallback(GLFWwindow* window_impl, int pathc, const char** pathv)
{
    auto window = reinterpret_cast<GLFW::Window*>(glfwGetWindowUserPointer(window_impl));
    std::vector<std::filesystem::path> paths(pathv, pathv + pathc);
    window->on_drop(paths);
}

static void scrollCallback(GLFWwindow* window_impl, double xoffset, double yoffset)
{
    auto window = reinterpret_cast<GLFW::Window*>(glfwGetWindowUserPointer(window_impl));
    window->on_scroll({ xoffset, yoffset });
}

// static void charModsCallback(GLFWwindow* window_impl, uint32_t codepoint, int mods)
// {
//     auto window = reinterpret_cast<GLFW::Window*>(glfwGetWindowUserPointer(window_impl));
//     window->on_char_mods(static_cast<char32_t>(codepoint), mods);
// }

static void cursorPosCallback(GLFWwindow* window_impl, double xpos, double ypos)
{
    auto window = reinterpret_cast<GLFW::Window*>(glfwGetWindowUserPointer(window_impl));
    window->on_mouse_move({ xpos, ypos });
}

static void windowPosCallback(GLFWwindow* window_impl, int xpos, int ypos)
{
    auto window = reinterpret_cast<GLFW::Window*>(glfwGetWindowUserPointer(window_impl));
    window->on_window_move({ xpos, ypos });
}

static void framebufferSizeCallback(GLFWwindow* window_impl, int xsize, int ysize)
{
    auto window = reinterpret_cast<GLFW::Window*>(glfwGetWindowUserPointer(window_impl));
    window->on_window_resize({ xsize, ysize });
}

static void mouseButtonCallback(GLFWwindow* window_impl, int button, int action, int mods)
{
    auto window = reinterpret_cast<GLFW::Window*>(glfwGetWindowUserPointer(window_impl));
    switch (action) {
    case GLFW_PRESS:
        window->on_mouse_button_press(button, mods);
        break;
    case GLFW_RELEASE:
        window->on_mouse_button_release(button, mods);
        break;
    }
}

void GLFW::Window::setup_callbacks()
{
    glfwSetKeyCallback(m_impl_window, keyCallback);
    glfwSetCharCallback(m_impl_window, charCallback);
    glfwSetDropCallback(m_impl_window, dropCallback);
    glfwSetScrollCallback(m_impl_window, scrollCallback);
    // glfwSetCharModsCallback(m_impl_window, charModsCallback); // scheluded for removal
    // glfwSetJoystickCallback() // not on a window
    glfwSetCursorPosCallback(m_impl_window, cursorPosCallback);
    glfwSetWindowPosCallback(m_impl_window, windowPosCallback);
    glfwSetFramebufferSizeCallback(m_impl_window, framebufferSizeCallback);
    glfwSetMouseButtonCallback(m_impl_window, mouseButtonCallback);
}