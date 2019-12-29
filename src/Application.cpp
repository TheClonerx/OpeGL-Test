#include "Application.hpp"

#include <cstddef>
#include <filesystem>
#include <fstream>

#include <iostream>
#include <string_view>

using namespace std::literals;

static void GLAPIENTRY MessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char *message, const void *param)
{
    std::cerr << "GL CALLBACK: ";
    if (type == GL_DEBUG_TYPE_ERROR)
        std::cerr << "\033[31m\033[5m\033[1m** GL ERROR **\033[0m\033[39m ";
    std::cerr << "type = 0x" << std::hex << type;
    std::cerr << " severity = 0x" << severity;
    std::cerr << "\n\t" << std::string_view{message, static_cast<std::size_t>(length)} << '\n';
}

std::string get_file(const std::string &path)
{
    std::ifstream file{path.c_str()};
    if (!file.is_open())
        throw std::runtime_error("Can't open file " + path);
    auto file_size = std::filesystem::file_size(path);
    std::string content(file_size, '\0');
    file.read(content.data(), file_size);
    return content;
}

GLFW::Window &Application::window() noexcept
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

    struct Vertex
    {
        glm::vec3 pos;
        glm::u8vec4 color;
        glm::vec2 uv;
    };

    Vertex vertices[]{
        {{+0.0, +0.5, +0.0}, {255u, 255u, 255u, 255u}, {0.5, 1.0}},
        {{-0.5, -0.5, -0.5}, {255u, 255u, 255u, 255u}, {0.0, 0.0}},
        {{+0.5, -0.5, -0.5}, {255u, 255u, 255u, 255u}, {1.0, 0.0}},

        {{+0.0, +0.5, +0.0}, {255u, 255u, 255u, 255u}, {0.5, 1.0}},
        {{+0.5, -0.5, -0.5}, {255u, 255u, 255u, 255u}, {0.0, 0.0}},
        {{+0.5, -0.5, +0.5}, {255u, 255u, 255u, 255u}, {1.0, 0.0}},

        {{+0.0, +0.5, +0.0}, {255u, 255u, 255u, 255u}, {0.5, 1.0}},
        {{+0.5, -0.5, +0.5}, {255u, 255u, 255u, 255u}, {0.0, 0.0}},
        {{-0.5, -0.5, +0.5}, {255u, 255u, 255u, 255u}, {1.0, 0.0}},

        {{+0.0, +0.5, +0.0}, {255u, 255u, 255u, 255u}, {0.5, 1.0}},
        {{-0.5, -0.5, +0.5}, {255u, 255u, 255u, 255u}, {0.0, 0.0}},
        {{-0.5, -0.5, -0.5}, {255u, 255u, 255u, 255u}, {1.0, 0.0}},

        // base
        {{-0.5, -0.5, +0.5}, {255u, 255u, 255u, 255u}, {0.0, 1.0}},
        {{+0.5, -0.5, +0.5}, {255u, 255u, 255u, 255u}, {1.0, 1.0}},
        {{-0.5, -0.5, -0.5}, {255u, 255u, 255u, 255u}, {0.0, 0.0}},
        {{+0.5, -0.5, -0.5}, {255u, 255u, 255u, 255u}, {1.0, 0.0}},
    };

    uint32_t indices[]{
        0, 2, 1,
        3, 5, 4,
        6, 8, 7,
        9, 11, 10,

        11 + 1, 11 + 2, 11 + 3,
        11 + 2, 11 + 4, 11 + 3};

    vertex_array.create();
    vertex_buffer.create();
    element_buffer.create();

    vertex_array.bind();

    vertex_buffer.bind();
    vertex_buffer.data(vertices, GL_STATIC_DRAW);

    element_buffer.bind();
    element_buffer.data(indices, GL_STATIC_DRAW);

    // coords (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)(offsetof(Vertex, pos)));
    glEnableVertexAttribArray(0);

    // color (r, g, b, a)
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (const void *)(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);

    // texture coords (u, v)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(2);

    texture1.create();
    texture1.bind();
    texture1.parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture1.parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    texture1.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture1.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture1.image(0, m_pinera);
}

void Application::setup()
{
    m_window.create({800, 600}, "OpenGL-Test");
    m_window.makeContextCurrent();
    if (!m_tcx_image.loadFromFile("assets/theclonerx.png"))
        throw std::runtime_error("Can't load theclonerx image");

    if (!m_pinera.loadFromFile("assets/avatar-pinera.png"))
        throw std::runtime_error("Can't load pinera image");

    m_window.setIcon(m_tcx_image);

    m_tcx_image.flipVertically(); // for opengl
    m_pinera.flipVertically();

    setupOpenGL();
}
