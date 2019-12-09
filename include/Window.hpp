#pragma once

#include <chrono>
#include <filesystem>
#include <glm/vec2.hpp>
#include <string_view>
#include <vector>

struct GLFWwindow;

namespace tcx {

class Image;

class Window {

public:
    Window() noexcept = default;
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&& rhs) noexcept;
    Window& operator=(Window&& rhs) noexcept;

    ~Window();

private:
    void setup_callbacks();

public:
    bool is_open();
    void create(glm::ivec2 size, std::string_view title);
    void destroy();

    void display();
    void update();
    void makeContextCurrent();
    void setIcon(const Image&);
    glm::ivec2 size() const noexcept;

    // events

    virtual void on_key_press(int key, int mod);
    virtual void on_key_release(int key, int mod);
    virtual void on_key_repeat(int key, int mod);
    virtual void on_char(char32_t);
    virtual void on_drop(const std::vector<std::filesystem::path>& paths);
    virtual void on_scroll(glm::dvec2 offset);
    virtual void on_mouse_move(glm::dvec2 new_pos);
    virtual void on_window_move(glm::ivec2 new_pos);
    virtual void on_window_resize(glm::ivec2 new_size);

private:
    GLFWwindow* m_impl_window = nullptr;
};

} // namespace tcx
