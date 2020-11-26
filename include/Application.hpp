#pragma once

#include <OpenGL/Buffer.hpp>
#include <OpenGL/ShaderProgram.hpp>
#include <OpenGL/Texture.hpp>
#include <OpenGL/VertexArray.hpp>
#include <SDL.h>
#include <SFML/Graphics/Image.hpp>
#include <Utilities.hpp>

class Application {
public:
    void render();
    void update(double);
    void setup();
    void on_event(SDL_Event const&);
    SDL_Window* window() noexcept;
    bool needs_redraw() const noexcept;

private:
    void setupOpenGL();

private:
    OpenGL::ShaderProgram shader_program;
    OpenGL::VertexArray vertex_array;
    OpenGL::Buffer<GL_ARRAY_BUFFER> vertex_buffer;
    OpenGL::Buffer<GL_ELEMENT_ARRAY_BUFFER> element_buffer;
    OpenGL::Texture texture1;
    sf::Image m_tcx_image;
    sf::Image m_pinera;

    // window stuff
    SDL_Window* m_window;
    SDL_GLContext m_glctx;

    bool m_needs_redraw = true;
    std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> m_imgui_info;
};
