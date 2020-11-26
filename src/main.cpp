#include <Application.hpp>
#include <OpenGL/GL.hpp>
#include <chrono>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>
#include <iostream>

template <typename Clock>
static double now_as_seconds() noexcept
{
    return std::chrono::duration_cast<std::chrono::duration<double>>(Clock::now().time_since_epoch()).count();
}

int main()
{
    using clock_type = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock, std::chrono::steady_clock>;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::fprintf(stderr, "SDL Error: %s\n", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }

    Application app;
    app.setup();

    double last_update = now_as_seconds<clock_type>();
    double delta = 0;

    bool running = true;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(app.window(), nullptr);
    ImGui_ImplOpenGL3_Init(nullptr);

    glClearColor(0, 0.25, 0.5, 1.0);
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            app.on_event(event);

            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(app.window()))
                running = false;

            if (!running)
                break;
        }
        if (!running)
            break;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(app.window());
        ImGui::NewFrame();

        app.update(delta);
        if (!running)
            break;

        bool needs_redraw = app.needs_redraw();

        if (needs_redraw)
            app.render();

        ImGui::EndFrame();

        if (needs_redraw) {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(app.window());
        }

        double now = now_as_seconds<clock_type>();
        delta = now - last_update;
        last_update = now;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_Quit();
}