#include <Application.hpp>
#include <imgui.h>
#include <iostream>
#include <array>

static double elapsed;
static float fov = glm::radians(90.0f);
static int max_frame_rate = 60;
static double last_frame;

void Application::update(double delta)
{
    if (ImGui::Begin("Test"))
    {
        ImGui::SliderAngle("FOV", &fov, 1);
        ImGui::SliderInt("Max Frame Rate", &max_frame_rate, 5, 145, max_frame_rate == 145 ? " Unlimited" : "%d fps");
    }
    ImGui::End();

    elapsed += delta;

    if (max_frame_rate == 145 || elapsed - last_frame >= 1.0 / max_frame_rate)
    {
        m_needs_redraw = true;
        last_frame = elapsed;
    }
    else
        m_needs_redraw = false;
}

#include <glm/gtx/io.hpp>

void Application::render()
{
    glDepthFunc(GL_LESS);
    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glClearColor(0, 0.25, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_program.bind();
    vertex_array.bind();

    glActiveTexture(GL_TEXTURE0);
    texture1.bind();

    glm::ivec4 viewport;
    glGetIntegerv(GL_VIEWPORT, &viewport.x);

    glm::mat4 model = glm::rotate(glm::mat4{1.0}, glm::radians(static_cast<float>(elapsed * 100)), glm::vec3{0.0, 1.0, 0.0});
    glm::mat4 view = glm::translate(glm::mat4{1.0}, glm::vec3{0.0, 0.0, -3.0});
    glm::mat4 projection = glm::perspectiveFov(fov, static_cast<float>(viewport.z), static_cast<float>(viewport.w), 0.1f, 100.0f);

    shader_program.uniform("modelMatrix", model);
    shader_program.uniform("viewMatrix", view);
    shader_program.uniform("projectionMatrix", projection);
    shader_program.uniform("texture1", 0);

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
}
