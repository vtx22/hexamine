#include "blockplot.hpp"

#include <SFML/Graphics.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "imgui-SFML.h"
#include "imgui.h"

int main(int argc, char *argv[])
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "hexamine - binary file viewer");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    Blockplot bp(&window);

    if (argc >= 2)
    {
        bp.set_file_path(argv[1]);
        bp.refresh_file();
    }

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        static float zoom = 0.3;
        static float zoom_speed = 0.02;

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseWheelMoved)
            {
                zoom += event.mouseWheel.delta * zoom_speed;

                if (zoom > 1)
                {
                    zoom = 1;
                }
                if (zoom < 0)
                {
                    zoom = 0;
                }

                bp.set_zoom(zoom);
            }
            if (event.type == sf::Event::MouseMoved)
            {
                bp.set_mouse_pos(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        if (ImGui::BeginMainMenuBar())
        {
            bp.set_main_menu_height((uint16_t)ImGui::GetWindowSize().y);
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O"))
                { /* Do stuff */
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        };
        ImGui::Begin("Settings");

        ImGui::SliderFloat("Zoom speed", &zoom_speed, 0.001, 0.1);

        if (ImGui::SliderFloat("Zoom", &zoom, 0, 1))
        {
            bp.set_zoom(zoom);
        }
        static int bpr = 32;
        if (ImGui::InputInt("Blocks/Row", &bpr))
        {
            bp.set_blocks_per_row(bpr);
        }
        static bool enable_highlight = true;
        if (ImGui::Checkbox("Hover highlight", &enable_highlight))
        {
        }

        ImGui::Separator();
        if (ImGui::Button("Update Binary"))
        {
            bp.refresh_file();
        }
        static bool auto_update = false;

        if (ImGui::Checkbox("Auto Update", &auto_update))
        {
            bp.set_auto_update(auto_update);
        }
        ImGui::End();

        window.clear(sf::Color(20, 20, 20));
        bp.update();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}