#include <Application.hpp>
#include <SDL.h>
#include <array>
#include <glm/gtx/io.hpp>
#include <imgui.h>
#include <iostream>
#include <ratio>

static double elapsed;
static float fov = glm::radians(60.0f);
static int max_frame_rate = 60;
static double last_frame;

static bool debug_windows = true;

static glm::vec4 clear_color {};
void Application::update(double delta)
{
    if (debug_windows) {
        if (ImGui::Begin("Debug###DEBUG_WINDOW", &debug_windows)) {
            ImGui::SliderAngle("FOV", &fov, 1, 180, "%.3f deg");
            ImGui::SliderInt("Max Frame Rate", &max_frame_rate, 5, 145, max_frame_rate == 145 ? " Unlimited" : "%d fps");
            if (ImGui::ColorPicker4("Clear Color", glm::value_ptr(clear_color)))
                glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            ImGui::End();

            if (ImGui::Begin("ImGui INI", &debug_windows)) {
                for (const auto& [section, values] : m_imgui_info) {
                    if (ImGui::TreeNode(&section, "[%s]", section.c_str())) {
                        for (const auto& [key, value] : values) {
                            ImGui::TextColored(ImVec4 { 0.1f, 0.3f, 0.7f, 1.0f }, "%s", key.c_str());
                            ImGui::SameLine(0, 0);
                            ImGui::TextUnformatted(" = ");
                            ImGui::SameLine(0, 0);
                            ImGui::TextColored(ImVec4 { 0.8f, 0.4f, 0.3f, 1.0f }, "%s", value.c_str());
                        }
                        ImGui::TreePop();
                    }
                }
            }
            ImGui::End();
        }
    }

    elapsed += delta;

    // we don't actually sleep, to keep updating on user input,
    // even though they may not see the effects just yet
    if (max_frame_rate == 145 || elapsed - last_frame >= 1.0 / max_frame_rate) {
        m_needs_redraw = true;
        last_frame = elapsed;
    } else {
        m_needs_redraw = false;
    }
}

void Application::on_event(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_F3) {
            debug_windows = !debug_windows;
        }
    }
}

void Application::render()
{
    glDepthFunc(GL_LESS);
    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);

    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

    glDisable(GL_SCISSOR_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glAlphaFunc(GL_EQUAL, 0.0f); // discard pixels with alpha equal to 0.0f
    glEnable(GL_BLEND);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_program.bind();
    vertex_array.bind();

    glActiveTexture(GL_TEXTURE0);
    texture1.bind();

    glm::ivec4 viewport;
    glGetIntegerv(GL_VIEWPORT, &viewport.x);

    glm::mat4 model = glm::rotate(glm::mat4 { 1.0f }, glm::radians(static_cast<float>(elapsed * 100)), glm::vec3 { 0.0, 1.0, 0.0 });
    glm::mat4 projection = glm::perspectiveFov(fov, static_cast<float>(viewport.z), static_cast<float>(viewport.w), 0.1f, 100.0f);
    glm::mat4 view { 1.0f };

    shader_program.uniform("modelMatrix", model);
    shader_program.uniform("viewMatrix", view);
    shader_program.uniform("projectionMatrix", projection);
    shader_program.uniform("texture1", 0);

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
}
