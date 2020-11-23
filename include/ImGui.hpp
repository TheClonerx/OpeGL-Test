#pragma once

#include <imgui.h>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

ImGuiContext* ImGui_Initialize();
void ImGui_Render(ImGuiContext* imgui_context);
void ImGui_Update(ImGuiContext* imgui_context, sf::Window& window, sf::RenderTarget& target, float delta);
void ImGui_Event(ImGuiContext* imgui_context, sf::Event& event);

inline void ImGui_Update(ImGuiContext* imgui_context, sf::RenderWindow& window, float delta)
{
    ImGui_Update(imgui_context, window, window, delta);
}