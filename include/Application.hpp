#pragma once
#include <OpenGL/Buffer.hpp>
#include <OpenGL/GL.hpp>
#include <OpenGL/ShaderProgram.hpp>
#include <OpenGL/Texture.hpp>
#include <OpenGL/VertexArray.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Application {
public:
    void start();

protected:
    void render();
    void update(double);
    void processEvent(const sf::Event&);

    void setWindowTitle(std::string);
    const std::string& getWindowTitle() const;

    void toggleFullscreen();
    void createWindow(sf::Vector2u, std::string, uint32_t);

    void setupWindow();
    void setupOpenGL();
    void loop();

private:
    OpenGL::ShaderProgram shader_program;
    OpenGL::VertexArray vertex_array;
    OpenGL::Buffer<GL_ARRAY_BUFFER> vertex_buffer;
    OpenGL::Buffer<GL_ELEMENT_ARRAY_BUFFER> element_buffer;
    OpenGL::Texture texture1;

    sf::Image m_tcx_image;

    // window stuff
    sf::RenderWindow m_window;

    std::string m_window_title;
    sf::Vector2u m_prev_size;
    uint32_t m_prev_style;
    bool is_fullscreen = false;

    //sfml stuff
    int sf_shader, sf_vao;
};