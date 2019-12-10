#pragma once
#include <GLFW/Window.hpp>

struct ImGuiIO;
struct ImGuiContext;

namespace ImGui {

class Window : public GLFW::Window {
public:
    Window();
    virtual ~Window();

    void on_create() override;
    void before_update() override;
    void on_update() override;
    void on_scroll(glm::dvec2 delta) override;
    void on_key_press(int key, int mods) override;
    void on_key_release(int key, int mods) override;
    void on_char(char32_t c) override;
    void on_close();

private:
    void update_mouse(ImGuiIO& io);
    void update_cursor(ImGuiIO& io);

private:
    ImGuiContext* m_imgui_context;
};

} // namespace tcx
