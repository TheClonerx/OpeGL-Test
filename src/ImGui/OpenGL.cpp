#include <OpenGL/GL.hpp>
#include <imgui.h>
#include <string_view>

using namespace std::literals;

static const auto g_imgui_vertex_shader_source = R"GLSL(
    #version 330 core
    layout (location = 0) in vec2 v_position;
    layout (location = 1) in vec2 v_uv;
    layout (location = 2) in vec4 v_color;

    uniform mat4 v_projection_matrix;

    out vec2 f_uv;
    out vec4 f_color;

    void main() {
        f_uv = v_uv;
        f_color = v_color;
        gl_Position = v_projection_matrix * vec4(v_position, 0, 1);
    }
)GLSL"sv;

static const auto g_imgui_fragment_shader_source = R"GLSL(
    #version 330 core

    in vec2 f_uv;
    in vec4 f_color;

    uniform sampler2D texture0;

    out vec4 out_color;

    void main() {
        out_color = f_color * texture(texture0, f_uv);
    }
)GLSL"sv;

#include <OpenGL/Buffer.hpp>
#include <OpenGL/Shader.hpp>
#include <OpenGL/ShaderProgram.hpp>
#include <OpenGL/Texture.hpp>
#include <OpenGL/VertexArray.hpp>

#include <spdlog/spdlog.h>

struct ImGui_Renderer_Data {
    OpenGL::ShaderProgram m_shader_program;
    OpenGL::Texture m_font_texture;
    OpenGL::VertexArray m_vertex_array;
    OpenGL::Buffer<GL_ARRAY_BUFFER> m_vertex_buffer;
    OpenGL::Buffer<GL_ELEMENT_ARRAY_BUFFER> m_index_buffer;
};

static bool ImGui_Setup_Shaders(ImGuiIO& io)
{
    ImGui_Renderer_Data& imgui_renderer_data = *reinterpret_cast<ImGui_Renderer_Data*>(io.BackendRendererUserData);
    OpenGL::VertexShader vertex_shader;
    OpenGL::FragmentShader fragment_shader;

    vertex_shader.create();
    fragment_shader.create();
    imgui_renderer_data.m_shader_program.create();

    vertex_shader.source(g_imgui_vertex_shader_source);
    fragment_shader.source(g_imgui_fragment_shader_source);

    if (!vertex_shader.compile()) {
        spdlog::error("IMGUI-BINDINGS: Cannot compile vertex shader {}", vertex_shader.info_log());
        return false;
    }

    if (!fragment_shader.compile()) {
        spdlog::error("IMGUI-BINDINGS: Cannot compile fragment shader {}", vertex_shader.info_log());
        return false;
    }

    imgui_renderer_data.m_shader_program.attach(vertex_shader, fragment_shader);

    if (!imgui_renderer_data.m_shader_program.link()) {
        spdlog::error("IMGUI-BINDINGS: Cannot link shader program {}", vertex_shader.info_log());
        return false;
    }

    return true;
}

static bool ImGui_Setup_FontsTexture(ImGuiIO& io)
{
    ImGui_Renderer_Data& imgui_renderer_data = *reinterpret_cast<ImGui_Renderer_Data*>(io.BackendRendererUserData);
    uint8_t* pixels;
    int width, height;

    constexpr auto font_path = "assets/fonts/Noto/NotoMono-Regular.ttf"sv;
    if (!io.Fonts->AddFontFromFileTTF(font_path.data(), 15)) {
        spdlog::error("IMGUI-BINDINGS: Can't load font {}", font_path);
        return false;
    }
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    imgui_renderer_data.m_font_texture.create();
    imgui_renderer_data.m_font_texture.bind();
    imgui_renderer_data.m_font_texture.parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    imgui_renderer_data.m_font_texture.parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    imgui_renderer_data.m_font_texture.image(0, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    io.Fonts->TexID = reinterpret_cast<ImTextureID>(static_cast<std::uintptr_t>(imgui_renderer_data.m_font_texture.handle()));

    return true;
}

static bool ImGui_Setup_Buffers(ImGuiIO& io)
{
    ImGui_Renderer_Data& imgui_renderer_data = *reinterpret_cast<ImGui_Renderer_Data*>(io.BackendRendererUserData);
    imgui_renderer_data.m_vertex_array.create();
    imgui_renderer_data.m_vertex_buffer.create();
    imgui_renderer_data.m_index_buffer.create();

    imgui_renderer_data.m_vertex_array.bind();
    imgui_renderer_data.m_vertex_buffer.bind();
    imgui_renderer_data.m_index_buffer.bind();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), reinterpret_cast<void*>(offsetof(ImDrawVert, pos)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), reinterpret_cast<void*>(offsetof(ImDrawVert, uv)));
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), reinterpret_cast<void*>(offsetof(ImDrawVert, col)));

    return true;
}

#include <glm/mat4x4.hpp>

static void ImGui_ImplOpenGL3_SetupRenderState(ImGui_Renderer_Data& imgui_renderer_data, ImDrawData* draw_data, int fb_width, int fb_height)
{
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);

    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

    glm::mat4 ortho_projection = glm::ortho(L, R, B, T);

    imgui_renderer_data.m_shader_program.bind();

    glActiveTexture(GL_TEXTURE0);
    imgui_renderer_data.m_shader_program.uniform("texture0", 0);
    imgui_renderer_data.m_font_texture.bind();

    imgui_renderer_data.m_shader_program.uniform("v_projection_matrix", ortho_projection);

    imgui_renderer_data.m_vertex_array.bind();
}

void ImGui_RenderDrawData(ImDrawData* draw_data)
{
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;

    ImGuiIO& io = ImGui::GetIO();
    ImGui_Renderer_Data& imgui_renderer_data = *reinterpret_cast<ImGui_Renderer_Data*>(io.BackendRendererUserData);

    ImGui_ImplOpenGL3_SetupRenderState(imgui_renderer_data, draw_data, fb_width, fb_height);

    ImVec2 clip_off = draw_data->DisplayPos; // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    bool clip_origin_lower_left = true;
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
    GLenum last_clip_origin = 0;
    glGetIntegerv(GL_CLIP_ORIGIN, (GLint*)&last_clip_origin); // Support for GL 4.5's glClipControl(GL_UPPER_LEFT)
    if (last_clip_origin == GL_UPPER_LEFT)
        clip_origin_lower_left = false;
#endif

    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];

        // Upload vertex/index buffers
        imgui_renderer_data.m_vertex_buffer.data(cmd_list->VtxBuffer, GL_STREAM_DRAW);
        imgui_renderer_data.m_index_buffer.data(cmd_list->IdxBuffer, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != NULL) {
                // User callback, registered via ImDrawList::AddCallback()
                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    ImGui_ImplOpenGL3_SetupRenderState(imgui_renderer_data, draw_data, fb_width, fb_height);
                else
                    pcmd->UserCallback(cmd_list, pcmd);
            } else {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f) {
                    // Apply scissor/clipping rectangle
                    if (clip_origin_lower_left)
                        glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
                    else
                        glScissor((int)clip_rect.x, (int)clip_rect.y, (int)clip_rect.z, (int)clip_rect.w); // Support for GL 4.5 rarely used glClipControl(GL_UPPER_LEFT)

                    // Draw
                    glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)));
                }
            }
        }
    }
}

bool ImGui_SetupOpenGL()
{
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = "imgui_tcx_impl_opengl";
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
    io.BackendRendererUserData = new ImGui_Renderer_Data;

    if (!ImGui_Setup_Shaders(io))
        return false;

    if (!ImGui_Setup_FontsTexture(io))
        return false;

    if (!ImGui_Setup_Buffers(io))
        return false;
    return true;
}

void ImGui_CleanUp_OpenGL()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui_Renderer_Data* imgui_renderer_data = reinterpret_cast<ImGui_Renderer_Data*>(io.BackendRendererUserData);
    delete imgui_renderer_data; // calls opengl objects destructors too
}