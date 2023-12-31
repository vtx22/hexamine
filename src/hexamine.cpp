#include "blockplot.hpp"

#include <SFML/Graphics.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "imgui-SFML.h"
#include "imgui.h"

#define VERSION "v1.0"

int main(int argc, char *argv[])
{
    std::string win_title = std::string("hexamine - ") + VERSION;
    sf::RenderWindow window(sf::VideoMode(1280, 720), win_title);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
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

            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
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
        /*
        if (ImGui::BeginMainMenuBar())
        {
            bp.set_main_menu_height((uint16_t)ImGui::GetWindowSize().y);
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O"))
                {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        */
        ImGui::Begin("Settings");
        ImGui::Text("Viewport");
        ImGui::SliderFloat("Zoom speed", &zoom_speed, 0.001, 0.1);

        if (ImGui::SliderFloat("Zoom", &zoom, 0, 1))
        {
            bp.set_zoom(zoom);
        }
        static int bpr = 32;
        ImGui::Text("Block Display");
        if (ImGui::InputInt("Blocks/Row", &bpr))
        {
            bp.set_blocks_per_row(bpr);
        }
        static int offs = 32;
        ImGui::Text("Block Offset");
        if (ImGui::InputInt("Offset", &offs))
        {
            bp.set_block_offset((uint32_t)offs);
        }
        static bool enable_highlight = true;
        if (ImGui::Checkbox("Hover highlight", &enable_highlight))
        {
            bp.set_hover_highlight(enable_highlight);
        }

        ImGui::Separator();
        ImGui::Text("File Reading");
        if (ImGui::Button("Update Binary"))
        {
            bp.refresh_file();
        }
        static bool auto_update = false;
        ImGui::SameLine();

        if (ImGui::Checkbox("Auto Update", &auto_update))
        {
            bp.set_auto_update(auto_update);
        }
        ImGui::End();

        ImGui::Begin("File info");
        ImGui::Value("Total size", (unsigned int)bp.get_total_size());
        ImGui::Value("Last Update", (unsigned int)bp.get_last_edit());
        ImGui::Separator();
        uint8_t val = bp.get_selected_value();
        ImGui::Value("Selected Value", (unsigned int)val);
        ImGui::SameLine();
        std::string s = bp.get_ascii_string(val);
        ImGui::Text("(%s)", s.c_str());
        ImGui::Value("Selected Address", (unsigned int)bp.get_selected_address());
        ImGui::End();

        window.clear(sf::Color(20, 20, 20));
        bp.update();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}