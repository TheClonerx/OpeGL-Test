#pragma once
#include <ImGui/Window.hpp>
#include <Image.hpp>
#include <OpenGL/Buffer.hpp>
#include <OpenGL/ShaderProgram.hpp>
#include <OpenGL/Texture.hpp>
#include <OpenGL/VertexArray.hpp>

class GameWindow : public ImGui::Window {
    inline void on_window_resize(glm::ivec2 new_size) override
    {
        ImGui::Window::on_window_resize(new_size);
        glViewport(0, 0, new_size.x, new_size.y);
    }
};

class Application {
public:
    void render();
    void update(double);
    void setup();
    GLFW::Window& window() noexcept;
    bool needs_redraw() const noexcept;

private:
    void setupOpenGL();

private:
    OpenGL::ShaderProgram shader_program;
    OpenGL::VertexArray vertex_array;
    OpenGL::Buffer<GL_ARRAY_BUFFER> vertex_buffer;
    OpenGL::Buffer<GL_ELEMENT_ARRAY_BUFFER> element_buffer;
    OpenGL::Texture texture1;
    tcx::Image m_tcx_image;

    // window stuff
    GameWindow m_window;

    bool m_needs_redraw = true;
};