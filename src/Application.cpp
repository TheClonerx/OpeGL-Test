#include <Application.hpp>
#include <ImGui.hpp>
#include <Utilities.hpp>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

using namespace std::literals;

static void GLAPIENTRY MessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* param)
{
    std::cerr << "GL CALLBACK: ";
    if (type == GL_DEBUG_TYPE_ERROR)
        std::cerr << "\033[31m\033[5m\033[1m** GL ERROR **\033[0m\033[39m ";
    std::cerr << "type = 0x" << std::hex << type;
    std::cerr << " severity = 0x" << severity;
    std::cerr << "\n\t" << std::string_view { message, static_cast<std::size_t>(length) } << '\n';
}

std::string get_file(std::string_view path)
{
    std::ifstream file { path.data() };
    if (!file.is_open())
        throw std::runtime_error("Can't open file " + std::string { path });
    auto file_size = std::filesystem::file_size(path);
    std::string content(file_size, '\0');
    file.read(content.data(), file_size);
    return content;
}

sf::RenderWindow& Application::window() noexcept
{
    return m_window;
}

bool Application::needs_redraw() const noexcept
{
    return m_needs_redraw;
}

void Application::setupOpenGL()
{
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);
#endif

    {
        OpenGL::FragmentShader fragment_shader;
        OpenGL::VertexShader vertex_shader;
        fragment_shader.create();
        vertex_shader.create();

        fragment_shader.source(get_file("assets/fragment.glsl"));
        vertex_shader.source(get_file("assets/vertex.glsl"));

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
        { { +0.0, +0.5, +0.0 }, { 255u, 255u, 255u, 255u }, { 0.5, 1.0 } },
        { { -0.5, -0.5, -0.5 }, { 255u, 255u, 255u, 255u }, { 0.0, 0.0 } },
        { { +0.5, -0.5, -0.5 }, { 255u, 255u, 255u, 255u }, { 1.0, 0.0 } },

        { { +0.0, +0.5, +0.0 }, { 255u, 255u, 255u, 255u }, { 0.5, 1.0 } },
        { { +0.5, -0.5, -0.5 }, { 255u, 255u, 255u, 255u }, { 0.0, 0.0 } },
        { { +0.5, -0.5, +0.5 }, { 255u, 255u, 255u, 255u }, { 1.0, 0.0 } },

        { { +0.0, +0.5, +0.0 }, { 255u, 255u, 255u, 255u }, { 0.5, 1.0 } },
        { { +0.5, -0.5, +0.5 }, { 255u, 255u, 255u, 255u }, { 0.0, 0.0 } },
        { { -0.5, -0.5, +0.5 }, { 255u, 255u, 255u, 255u }, { 1.0, 0.0 } },

        { { +0.0, +0.5, +0.0 }, { 255u, 255u, 255u, 255u }, { 0.5, 1.0 } },
        { { -0.5, -0.5, +0.5 }, { 255u, 255u, 255u, 255u }, { 0.0, 0.0 } },
        { { -0.5, -0.5, -0.5 }, { 255u, 255u, 255u, 255u }, { 1.0, 0.0 } },

        // base
        { { -0.5, -0.5, +0.5 }, { 255u, 255u, 255u, 255u }, { 0.0, 1.0 } },
        { { +0.5, -0.5, +0.5 }, { 255u, 255u, 255u, 255u }, { 1.0, 1.0 } },
        { { -0.5, -0.5, -0.5 }, { 255u, 255u, 255u, 255u }, { 0.0, 0.0 } },
        { { +0.5, -0.5, -0.5 }, { 255u, 255u, 255u, 255u }, { 1.0, 0.0 } },
    };

    uint32_t indices[] {
        0, 2, 1,
        3, 5, 4,
        6, 8, 7,
        9, 11, 10,

        14, 13, 12,
        14, 15, 13
    };

    vertex_array.create();
    vertex_buffer.create();
    element_buffer.create();

    vertex_array.bind();

    vertex_buffer.bind();
    vertex_buffer.data(vertices, GL_STATIC_DRAW);

    element_buffer.bind();
    element_buffer.data(indices, GL_STATIC_DRAW);

    // coords (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, pos)));
    glEnableVertexAttribArray(0);

    // color (r, g, b, a)
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (const void*)(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);

    // texture coords (u, v)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(2);

    texture1.create();
    texture1.bind();
    texture1.parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture1.parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    texture1.parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture1.parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    texture1.image(0, GL_RGBA, m_pinera.getSize().x, m_pinera.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, m_pinera.getPixelsPtr());
}

void Application::setup()
{
    sf::ContextSettings context_settings;
    context_settings.majorVersion = 3;
    context_settings.minorVersion = 3;
    context_settings.depthBits = 24;
#ifndef NDEBUG
    context_settings.attributeFlags |= sf::ContextSettings::Attribute::Debug;
#endif

    m_window.create({ 800, 600 }, "OpenGL-Test", sf::Style::Default, context_settings);
    m_window.setActive();
    m_window.setKeyRepeatEnabled(false);
    if (int error = glewInit(); error != GLEW_OK) {
        std::fprintf(stderr, "Error initializing GLEW: (%d) %s\n", error, glewGetErrorString(error));
        std::exit(EXIT_FAILURE);
    }

    {
        tcx::TimeIt timer = "assets/theclonerx.png"sv;
        if (!m_tcx_image.loadFromFile("assets/theclonerx.png"))
            throw std::runtime_error("Can't load theclonerx image");
    }
    {
        tcx::TimeIt timer = "assets/avatar-pinera.png"sv;
        if (!m_pinera.loadFromFile("assets/avatar-pinera.png"))
            throw std::runtime_error("Can't load pinera image");
    }
    {
        tcx::TimeIt timer = "tcx::parse_ini(\"imgui.ini\");"sv;
        m_imgui_info = tcx::parse_ini("imgui.ini");
    }
    m_window.setIcon(m_tcx_image.getSize().x, m_tcx_image.getSize().y, m_tcx_image.getPixelsPtr());

    m_tcx_image.flipVertically(); // for opengl
    m_pinera.flipVertically();
    setupOpenGL();
}
