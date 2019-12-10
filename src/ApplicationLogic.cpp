#include "Application.hpp"
#include <SFML/Window/Event.hpp>
#include <imgui.h>
#include <iostream>

static double elapsed;
static float fov = glm::radians(90.0f);

void Application::update(double delta)
{
    if (ImGui::Begin("Test")) {
        ImGui::SliderAngle("FOV", &fov, 1);
    }
    ImGui::End();
    elapsed += delta;
    // std::cout << "Elapsed: " << elapsed << '\n';
}

#include <glm/gtx/io.hpp>

void Application::render()
{
    glClearColor(0, 0.25, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    shader_program.bind();
    vertex_array.bind();

    glActiveTexture(GL_TEXTURE0);
    texture1.bind();

    glm::ivec4 viewport;
    glGetIntegerv(GL_VIEWPORT, &viewport.x);

    glm::mat4 model = glm::rotate(glm::mat4 { 1.0 }, glm::radians(static_cast<float>(elapsed * 100)), glm::vec3 { 1.0, 0.0, 0.0 });
    glm::mat4 view = glm::translate(glm::mat4 { 1.0 }, glm::vec3 { 0.0, 0.0, -3.0 });
    glm::mat4 projection = glm::perspectiveFov(fov, static_cast<float>(viewport.z), static_cast<float>(viewport.w), 0.0f, 100.0f);

    shader_program.uniform("modelMatrix", model);
    shader_program.uniform("viewMatrix", view);
    shader_program.uniform("projectionMatrix", projection);
    shader_program.uniform("texture1", 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
