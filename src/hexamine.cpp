#include "blockplot.hpp"

#include <SFML/Graphics.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "imgui-SFML.h"
#include "imgui.h"

sf::Color background(20, 20, 20);

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
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
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
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                { /* Do stuff */
                }
                if (ImGui::MenuItem("Close", "Ctrl+W"))
                {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        };
        ImGui::Begin("Settings");

        static float zoom = 0;
        if (ImGui::SliderFloat("Zoom", &zoom, 0, 1))
        {
            bp.set_zoom(zoom);
        }
        static int bpr = 0;
        if (ImGui::InputInt("Blocks/Row", &bpr))
        {
            bp.set_blocks_per_row(bpr);
        }

        ImGui::Separator();
        if (ImGui::Button("Update Binary"))
        {
        }
        static bool auto_update = false;

        if (ImGui::Checkbox("Auto Update", &auto_update))
        {
        }
        ImGui::End();

        window.clear(background);
        bp.update();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}