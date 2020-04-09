#include <Application.hpp>
#include <ImGui.hpp>
#include <OpenGL/GL.hpp>
#include <SFML/Window/Event.hpp>
#include <chrono>
#include <iostream>

template <typename Clock>
static double now_as_seconds() noexcept
{
    return std::chrono::duration_cast<std::chrono::duration<double>>(Clock::now().time_since_epoch()).count();
}

int main()
/* try */ {
    using clock_t = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock, std::chrono::steady_clock>;

    Application app;
    app.setup();

    double last_update = now_as_seconds<clock_t>();
    double delta = 0;

    bool running = true;

    ImGuiContext* imgui_context = ImGui_Initialize();

    while (running) {
        sf::Event event;
        while (app.window().pollEvent(event)) {
            ImGui_Event(imgui_context, event);

            app.on_event(event);

            if (!app.window().isOpen()) { // window.close() may be called
                running = false;
            }
        }
        if (!running)
            break;

        ImGui_Update(imgui_context, app.window(), static_cast<float>(delta));

        ImGui::NewFrame();
        app.update(delta);
        if (!running)
            break;

        bool needs_redraw = app.needs_redraw();

        if (needs_redraw)
            app.render();

        ImGui::EndFrame();

        if (needs_redraw) {
            ImGui_Render(imgui_context); // draws to the active OpenGL context
            app.window().display();
        }

        double now = now_as_seconds<clock_t>();
        delta = now - last_update;
        last_update = now;
    }

    // the debugger is not showing the callstack of the exception
    // and for some reason, std::cerr and std::clog don't want to work.
    // so im just going to let the debugger automatically catch the exceptions

    // } catch (std::system_error& e) {
    //     std::cout << "Unhandled std::system_error:\n\t";
    //     std::cout << e.code().category().name() << ": (" << e.code().value() << ") " << e.code().message() << std::endl;
    //     throw;
    // } catch (std::exception& e) {
    //     std::cout << "Unhandled std::exception:\n\t";
    //     std::cout << e.what() << std::endl;
    //     throw;
    // } catch (...) {
    //     std::cout << "Unhandled unknown exception, rethrowing." << std::endl;
    //     throw;
    // }
}