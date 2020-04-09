#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Clipboard.hpp>
#include <SFML/Window/Cursor.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowBase.hpp>

#include <imgui.h>

static std::array<sf::Cursor, ImGuiMouseCursor_COUNT> g_MouseCursors {};
static bool g_MouseCursorsInitialized = false;
static double g_Time;

void ImGui_RenderDrawData(ImDrawData* draw_data);
bool ImGui_SetupOpenGL();
void ImGui_CleanUp_OpenGL();

static void initializeCursors()
{
    if (g_MouseCursorsInitialized)
        return;

    // they should default to array if the os doesn't support them

    g_MouseCursors[ImGuiMouseCursor_Arrow].loadFromSystem(sf::Cursor::Type::Arrow);
    g_MouseCursors[ImGuiMouseCursor_TextInput].loadFromSystem(sf::Cursor::Type::Text);
    g_MouseCursors[ImGuiMouseCursor_ResizeAll].loadFromSystem(sf::Cursor::Type::SizeAll);
    g_MouseCursors[ImGuiMouseCursor_ResizeNS].loadFromSystem(sf::Cursor::Type::SizeVertical);
    g_MouseCursors[ImGuiMouseCursor_ResizeEW].loadFromSystem(sf::Cursor::Type::SizeHorizontal);
    g_MouseCursors[ImGuiMouseCursor_ResizeNESW].loadFromSystem(sf::Cursor::Type::SizeBottomLeftTopRight);
    g_MouseCursors[ImGuiMouseCursor_ResizeNWSE].loadFromSystem(sf::Cursor::Type::SizeTopLeftBottomRight);
    g_MouseCursors[ImGuiMouseCursor_Hand].loadFromSystem(sf::Cursor::Type::Hand);
    g_MouseCursors[ImGuiMouseCursor_NotAllowed].loadFromSystem(sf::Cursor::Type::NotAllowed);

    g_MouseCursorsInitialized = true;
}

ImGuiContext* ImGui_Initialize()
{
    ImGuiContext* imgui_context = ImGui::CreateContext();
    ImGui::SetCurrentContext(imgui_context);

    ImGuiIO& io = ImGui::GetIO();

    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendPlatformName = "imgui_tcx_impl_for_sfml";

    static std::string clipboard_data;
    io.ClipboardUserData = &clipboard_data;
    io.SetClipboardTextFn = [](void* data, const char* text) {
        sf::Clipboard::setString(text);
    };
    io.GetClipboardTextFn = [](void* data) -> const char* {
        auto& clipboard_data = *reinterpret_cast<std::string*>(data);
        clipboard_data = sf::Clipboard::getString();
        return clipboard_data.c_str();
    };

    if (!ImGui_SetupOpenGL())
        throw std::runtime_error("ERROR:IMGUI:OPENGL: couldn't set up opengl for the imgui context");

    io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Key::Tab;
    io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Key::Left;
    io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Key::Right;
    io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Key::Up;
    io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Key::Down;
    io.KeyMap[ImGuiKey_PageUp] = sf::Keyboard::Key::PageUp;
    io.KeyMap[ImGuiKey_PageDown] = sf::Keyboard::Key::PageDown;
    io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Key::Home;
    io.KeyMap[ImGuiKey_End] = sf::Keyboard::Key::End;
    io.KeyMap[ImGuiKey_Insert] = sf::Keyboard::Key::Insert;
    io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Key::Delete;
    io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::Key::BackSpace;
    io.KeyMap[ImGuiKey_Space] = sf::Keyboard::Key::Space;
    io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Key::Enter;
    io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Key::Space;
    io.KeyMap[ImGuiKey_KeyPadEnter] = sf::Keyboard::Key::Enter; // there's no NumpadEnter in SFML sadly
    io.KeyMap[ImGuiKey_A] = sf::Keyboard::Key::A;
    io.KeyMap[ImGuiKey_C] = sf::Keyboard::Key::C;
    io.KeyMap[ImGuiKey_V] = sf::Keyboard::Key::V;
    io.KeyMap[ImGuiKey_X] = sf::Keyboard::Key::X;
    io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Key::Y;
    io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Key::Z;
    return imgui_context;
}

void ImGui_Render(ImGuiContext* imgui_context)
{
    ImGui::SetCurrentContext(imgui_context);
    ImGui::Render();
    if (auto data = ImGui::GetDrawData(); data)
        ImGui_RenderDrawData(data);
}

static void ImGui_Update_Mouse(ImGuiIO& io, sf::WindowBase& window);
static void ImGui_Update_Cursor(ImGuiIO& io, sf::WindowBase& window);
void ImGui_Update(ImGuiContext* imgui_context, sf::WindowBase& window, sf::RenderTarget& target, float delta)
{
    ImGui::SetCurrentContext(imgui_context);
    ImGuiIO& io = ImGui::GetIO();
    auto size = target.getSize();
    io.DisplaySize = { static_cast<float>(size.x), static_cast<float>(size.y) };
    io.DeltaTime = delta;
    ImGui_Update_Mouse(io, window);
    ImGui_Update_Cursor(io, window);
}

static void ImGui_Update_Mouse(ImGuiIO& io, sf::WindowBase& window)
{

    // ImGui Mouse buttons: 0=left, 1=right, 2=middle + 2 extras.
    // SFML  Mouse buttons: 0=left, 1=right, 2=middle, 3=xbutton1, 4=xbutton2
    for (size_t i = 0; i < std::size(io.MouseDown); ++i)
        io.MouseDown[i] = sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i));

    const ImVec2 mouse_pos_backup = io.MousePos;
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

    //if (glfwGetWindowAttrib(m_impl_window, GLFW_FOCUSED)) {
    if (io.WantSetMousePos) {
        sf::Mouse::setPosition(sf::Vector2i { static_cast<int>(mouse_pos_backup.x), static_cast<int>(mouse_pos_backup.y) }, window);
    } else {
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        io.MousePos = { static_cast<float>(pos.x), static_cast<float>(pos.y) };
    }
    //}
}

static void ImGui_Update_Cursor(ImGuiIO& io, sf::WindowBase& window)
{
    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) /*|| glfwGetInputMode(m_impl_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED*/)
        return;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    // we shouldn't use window.setMouseCursorVisible(bool)
    // becuase the user may want to do a different thing
    if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor) {
        window.setMouseCursorVisible(false);
    } else {
        window.setMouseCursor(g_MouseCursors[imgui_cursor]);
        window.setMouseCursorVisible(true);
    }
}

static void ImGui_Event_MouseWheelScrolled(ImGuiIO& io, sf::Event::MouseWheelScrollEvent& event)
{
    io.MouseWheelH += event.x;
    io.MouseWheel += event.y;
}

static void ImGui_Event_KeyPressed(ImGuiIO& io, sf::Event::KeyEvent& event)
{
    io.KeysDown[event.code] = true;

    io.KeyCtrl = event.control;
    io.KeyShift = event.shift;
    io.KeyAlt = event.alt;
    io.KeySuper = event.system; // do imgui really needs this button?
}

static void ImGui_Event_KeyReleased(ImGuiIO& io, sf::Event::KeyEvent& event)
{
    io.KeysDown[event.code] = false;

    io.KeyCtrl = event.control;
    io.KeyShift = event.shift;
    io.KeyAlt = event.alt;
    io.KeySuper = event.system;
}

static void ImGui_Event_TextEntered(ImGuiIO& io, sf::Event::TextEvent& event)
{
    io.AddInputCharacter(event.unicode); // UTF-32
}

void ImGui_Event(ImGuiContext* imgui_context, sf::Event& event)
{
    ImGui::SetCurrentContext(imgui_context);
    ImGuiIO& io = ImGui::GetIO();

    switch (event.type) {
    case sf::Event::EventType::MouseWheelScrolled:
        ImGui_Event_MouseWheelScrolled(io, event.mouseWheelScroll);
    case sf::Event::EventType::KeyPressed:
        ImGui_Event_KeyPressed(io, event.key);
    case sf::Event::EventType::KeyReleased:
        ImGui_Event_KeyReleased(io, event.key);
    case sf::Event::EventType::TextEntered:
        ImGui_Event_TextEntered(io, event.text);
    }
}
