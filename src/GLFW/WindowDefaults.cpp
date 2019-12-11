#include <GLFW/Window.hpp>

void GLFW::Window::on_char(char32_t) {}
void GLFW::Window::on_drop(const std::vector<std::filesystem::path>&) {}
void GLFW::Window::on_key_press(int, int) {}
void GLFW::Window::on_key_release(int, int) {}
void GLFW::Window::on_key_repeat(int, int) {}
void GLFW::Window::on_mouse_move(glm::dvec2) {}
void GLFW::Window::on_scroll(glm::dvec2) {}
void GLFW::Window::on_window_move(glm::ivec2) {}
void GLFW::Window::on_window_resize(glm::ivec2) {}
void GLFW::Window::on_mouse_button_press(int, int) {}
void GLFW::Window::on_mouse_button_release(int, int) {}
void GLFW::Window::on_update() {}
void GLFW::Window::before_update() {}
void GLFW::Window::on_create() {}
void GLFW::Window::on_close() {}
void GLFW::Window::before_display() {}
void GLFW::Window::on_display() {}