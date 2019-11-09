#include "Application.hpp"
#include <SFML/Window/Event.hpp>

void Application::processEvent(const sf::Event& event)
{
    if (event.type == sf::Event::EventType::Closed)
        m_window.close();
    else if (event.type == sf::Event::EventType::Resized)
        glViewport(0, 0, event.size.width, event.size.height);
    else if (event.type == sf::Event::EventType::KeyPressed) {
        if (event.key.code == sf::Keyboard::Key::F11) {
            toggleFullscreen();
        }
    }
}

#include <glm.hpp>

static double elapsed;
static float fov = 90.0f;

#include <imgui.h>

void Application::update(double delta)
{
    if (ImGui::Begin("Test")) {
        ImGui::SliderAngle("FOV", &fov, 0);
    }
    ImGui::End();
    elapsed += delta;
}

void Application::render()
{
    glClearColor(0, 0.25, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    shader_program.bind();
    vertex_array.bind();

    glActiveTexture(GL_TEXTURE0);
    texture1.bind();

    glm::mat4 model = glm::rotate(glm::mat4 { 1.0 }, glm::radians(static_cast<float>(elapsed * 10)), glm::vec3 { 1.0, 0.0, 0.0 });
    glm::mat4 view = glm::translate(glm::mat4 { 1.0 }, glm::vec3 { 0.0, 0.0, -3.0 });
    glm::mat4 projection = glm::perspectiveFov(glm::radians(fov), (float)m_window.getSize().x, (float)m_window.getSize().y, 0.1f, 100.0f);

    shader_program.uniform("modelMatrix", model);
    shader_program.uniform("viewMatrix", view);
    shader_program.uniform("projectionMatrix", projection);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
