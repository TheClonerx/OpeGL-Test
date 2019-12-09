#include <Window.hpp>

void tcx::Window::on_char(char32_t) {}
void tcx::Window::on_drop(const std::vector<std::filesystem::path>&) {}
void tcx::Window::on_key_press(int, int) {}
void tcx::Window::on_key_release(int, int) {}
void tcx::Window::on_key_repeat(int, int) {}
void tcx::Window::on_mouse_move(glm::dvec2) {}
void tcx::Window::on_scroll(glm::dvec2) {}
void tcx::Window::on_window_move(glm::ivec2) {}
void tcx::Window::on_window_resize(glm::ivec2) {}