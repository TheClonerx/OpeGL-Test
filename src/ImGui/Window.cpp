#include <GLFW/glfw3.h>
#include <ImGui/Window.hpp>

#include <imgui.h>

static GLFWcursor* g_MouseCursors[ImGuiMouseCursor_COUNT] {};
static bool g_MouseCursorsInitialized = false;
static double g_Time;

void ImGui_RenderDrawData(ImDrawData* draw_data);
bool ImGui_SetupOpenGL();
void ImGui_CleanUp_OpenGL();

static void initializeCursors()
{
    if (g_MouseCursorsInitialized)
        return;
    g_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
#if GLFW_HAS_NEW_CURSORS
    g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
    // g_MouseCursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
#else
    g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    // g_MouseCursors[ImGuiMouseCursor_NotAllowed] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
#endif
    g_MouseCursorsInitialized = true;
}

ImGui::Window::Window()
    : GLFW::Window {}
    , m_imgui_context { ImGui::CreateContext() }
{
    ImGui::SetCurrentContext(m_imgui_context);

    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos; // We can honor io.WantSetMousePos requests (optional, rarely used)
    io.BackendPlatformName = "imgui_tcx_impl_for_glfw";

    uint8_t* data;
    glm::ivec2 size;

    io.SetClipboardTextFn = reinterpret_cast<void (*)(void*, const char*)>(glfwSetClipboardString);
    io.GetClipboardTextFn = reinterpret_cast<const char* (*)(void*)>(glfwGetClipboardString);

    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
}

ImGui::Window::~Window()
{
    if (m_imgui_context) {
        ImGui::DestroyContext(m_imgui_context);
        m_imgui_context = nullptr;
    }
}

void ImGui::Window::on_create()
{
    ImGui::SetCurrentContext(m_imgui_context);
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    io.UserData = m_impl_window;
    if (!ImGui_SetupOpenGL())
        throw std::runtime_error("ERROR:IMGUI:OPENGL: couldn't set up opengl for the imgui context");
    io.ClipboardUserData = m_impl_window;
    auto frame_size = size();
    io.DisplaySize = { static_cast<float>(frame_size.x), static_cast<float>(frame_size.y) };
    ImGui::NewFrame();
}

void ImGui::Window::before_display() {
    ImGui::SetCurrentContext(m_imgui_context);
    ImGui::Render();
    if (auto data = ImGui::GetDrawData(); data) {
        makeContextCurrent();
        ImGui_RenderDrawData(data);
    }
}

void ImGui::Window::before_update()
{
    ImGui::SetCurrentContext(m_imgui_context);
    ImGui::EndFrame();
}

void ImGui::Window::on_update()
{
    if (!m_impl_window)
        return;
    ImGuiIO& io = ImGui::GetIO();

    int w, h;
    int display_w, display_h;
    glfwGetWindowSize(m_impl_window, &w, &h);
    glfwGetFramebufferSize(m_impl_window, &display_w, &display_h);
    io.DisplaySize = { static_cast<float>(w), static_cast<float>(h) };
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = { static_cast<float>(display_w) / w, static_cast<float>(display_h) / h };
    double current_time = glfwGetTime();
    io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
    g_Time = current_time;

    update_mouse(io);
    update_cursor(io);

    ImGui::NewFrame();
}

void ImGui::Window::update_mouse(ImGuiIO& io)
{

    for (size_t i = 0; i < std::size(io.MouseDown); ++i)
        io.MouseDown[i] = glfwGetMouseButton(m_impl_window, i) == GLFW_PRESS;

    const ImVec2 mouse_pos_backup = io.MousePos;
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

    if (glfwGetWindowAttrib(m_impl_window, GLFW_FOCUSED)) {
        if (io.WantSetMousePos)
            glfwSetWindowPos(m_impl_window, mouse_pos_backup.x, mouse_pos_backup.y);
        else {
            glm::dvec2 pos;
            glfwGetCursorPos(m_impl_window, &pos.x, &pos.y);
            io.MousePos = { static_cast<float>(pos.x), static_cast<float>(pos.y) };
        }
    }
}

void ImGui::Window::update_cursor(ImGuiIO& io)
{
    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(m_impl_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
        glfwSetInputMode(m_impl_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    else {
        glfwSetCursor(m_impl_window, g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
        glfwSetInputMode(m_impl_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void ImGui::Window::on_scroll(glm::dvec2 delta)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH += delta.x;
    io.MouseWheel += delta.y;
}

void ImGui::Window::on_key_press(int key, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[key] = true;

    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

void ImGui::Window::on_key_release(int key, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[key] = false;

    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

void ImGui::Window::on_char(char32_t c)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(c);
}

void ImGui::Window::on_close()
{
    ImGui::SetCurrentContext(m_imgui_context);
    ImGuiIO& io = ImGui::GetIO();
    ImGui_CleanUp_OpenGL();
    ImGui::DestroyContext(m_imgui_context);
    m_imgui_context = nullptr;
}