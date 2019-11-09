#include "Application.hpp"
#include <GL/glew.h>
#include <SFML/Window/Event.hpp>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <imgui-SFML.h>
#include <iostream>
#include <string_view>

using namespace std::literals;

void Application::loop()
{
    sf::Event event;
    sf::Clock clock;
    ImGui::SFML::Init(m_window, static_cast<sf::Vector2f>(m_window.getSize()));
    while (m_window.isOpen()) {
        const sf::Time sf_delta = clock.restart();
        const double delta = sf_delta.asMicroseconds() / 1'000'000.0;
        while (m_window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            this->processEvent(event);
        }
        ImGui::SFML::Update(sf::Mouse::getPosition(m_window), static_cast<sf::Vector2f>(m_window.getSize()), sf_delta);
        this->update(delta);
        this->render();
        ImGui::SFML::Render();
        m_window.display();
    }
    ImGui::SFML::Shutdown();
}

void Application::setupWindow()
{
    createWindow({ 800, 600 }, "OpenGL-Test", sf::Style::Close | sf::Style::Resize);

    m_window.setKeyRepeatEnabled(false);
    m_window.setFramerateLimit(144);
    m_window.setVerticalSyncEnabled(true);
    m_window.setActive();

    if (int error = glewInit(); error != GLEW_OK) {
        std::cerr << "Error initializing GLEW\n";
        std::cerr << glewGetErrorString(error) << "\n";
    }
}

void GLAPIENTRY MessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* param)
{
    std::cerr << "GL CALLBACK: ";
    if (type == GL_DEBUG_TYPE_ERROR)
        std::cerr << "\033[31m\033[5m\033[1m** GL ERROR **\033[0m\033[39m ";
    std::cerr << "type = 0x" << std::hex << type;
    std::cerr << " severity = 0x" << severity;
    std::cerr << "\n\t" << std::string_view { message, static_cast<std::size_t>(length) } << '\n';
}

std::string get_file(const std::string& path)
{
    std::ifstream file { path.c_str() };
    if (!file.is_open())
        throw std::runtime_error("Can't open file " + path);
    auto file_size = std::filesystem::file_size(path);
    std::string content(file_size, '\0');
    file.read(content.data(), file_size);
    return content;
}

void Application::setupOpenGL()
{
    glGetIntegerv(GL_CURRENT_PROGRAM, &sf_shader);
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &sf_vao);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
        OpenGL::FragmentShader fragment_shader;
        OpenGL::VertexShader vertex_shader;
        fragment_shader.create();
        vertex_shader.create();

        fragment_shader.source(get_file("fragment.glsl"));
        vertex_shader.source(get_file("vertex.glsl"));

        if (!fragment_shader.compile())
            std::cerr << "Error compiling the fragment shader:\n\t" << fragment_shader.info_log() << '\n';
        if (!vertex_shader.compile())
            std::cerr << "Error compiling the vertex shader:\n\t" << vertex_shader.info_log() << '\n';

        shader_program.create();
        shader_program.attach(vertex_shader, fragment_shader);
    }
    if (!shader_program.link())
        std::cerr << "Error linking the shader program:\n\t" << shader_program.info_log() << '\n';

    struct Vertex {
        glm::vec3 pos;
        glm::u8vec4 color;
        glm::vec2 uv;
    };

    Vertex vertices[] {
        { { -0.5, -0.5, +0.0 }, { 255u, 255u, 255u, 255u }, { 0.0, 0.0 } }, // bottom left
        { { +0.5, -0.5, +0.0 }, { 255u, 255u, 255u, 255u }, { 1.0, 0.0 } }, // bottom right
        { { +0.5, +0.5, +0.0 }, { 255u, 255u, 255u, 255u }, { 1.0, 1.0 } }, // top right
        { { -0.5, +0.5, +0.0 }, { 255u, 255u, 255u, 255u }, { 0.0, 1.0 } } //  top left
    };

    uint32_t indices[] {
        0, 1, 2,
        2, 3, 0
    };

    vertex_array.create();
    vertex_buffer.create();
    element_buffer.create();

    vertex_array.bind();

    vertex_buffer.bind();
    vertex_buffer.data(vertices, GL_STATIC_DRAW);

    // coords (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(0);

    // color (r, g, b, a)
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (const void*)(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);

    // texture coords (u, v)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(2);

    element_buffer.bind();
    element_buffer.data(indices, GL_STATIC_DRAW);

    texture1.create();
    texture1.bind();
    texture1.parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture1.parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    texture1.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture1.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture1.image(0, m_tcx_image);

    shader_program.uniform("texture1", 0);
}

void Application::start()
{

    if (!m_tcx_image.loadFromFile("theclonerx.png"))
        throw std::runtime_error("Can't load tcx image");
    m_tcx_image.flipVertically();

    setupWindow();
    setupOpenGL();

    loop();
}

void Application::setWindowTitle(std::string title)
{
    m_window.setTitle(title);
    m_window_title = std::move(title);
}

const std::string& Application::getWindowTitle() const
{
    return m_window_title;
}

void Application::toggleFullscreen()
{
    m_window.close();
    if (is_fullscreen)
        m_window.create(sf::VideoMode { m_prev_size.x, m_prev_size.y }, m_window_title, m_prev_style);
    else
        m_window.create(sf::VideoMode::getFullscreenModes()[0], m_window_title, sf::Style::Fullscreen);
    m_window.setActive();
    is_fullscreen = !is_fullscreen;
}

void Application::createWindow(sf::Vector2u size, std::string title, uint32_t style)
{
    /*  sf::ContextSettings settings;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.antialiasingLevel = 0; */

    m_window.create(sf::VideoMode { size.x, size.y }, title, style /* , settings */);
    m_prev_size = m_window.getSize();
    m_prev_style = style;
    m_window_title = std::move(title);
}
