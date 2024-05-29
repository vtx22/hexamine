#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <stdint.h>

#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <commdlg.h>
#include <string.h>
#include <fstream>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "imgui-SFML.h"
#include "imgui.h"
#include "implot.h"

#include "util.h"
#include "file_reader.hpp"

#include "hexamine_config.h"
#include "imgui_memory_editor.h"

std::vector<uint8_t> data;
std::vector<std::string> x_ticks, y_ticks;
std::vector<const char *> xs_ticks, ys_ticks;
size_t actual_data_size = 0;

// Setting Flags
bool show_values = true;
bool grid_lines = true;
bool lock_aspect = true;
bool sync_mem_edit_cols = true;
int cols = 1;
int rows = 1;

int close_app(sf::RenderWindow &window);
void calculate_new_ticks();

int main(int argc, char *argv[])
{
    FileReader fr;

    // Get file from path if specified
    if (argc == 2)
    {
        const int res = fr.update_data(argv[1]);
        if (res != FR_SUCCESS)
        {
            std::cerr << "Could not open file " << argv[1] << " | Error: " << res << std::endl;
            return res;
        }

        data = fr.get_file_data();
        actual_data_size = data.size();
        cols = sqrt(actual_data_size);
        calculate_new_ticks();
    }

    sf::RenderWindow window(sf::VideoMode(1280, 720), std::string("hexamine - ") + HEXAMINE_VERSION);

    window.setFramerateLimit(HEXAMINE_MAX_FPS);
    window.setVerticalSyncEnabled(HEXAMINE_VSYNC);

    if (!ImGui::SFML::Init(window))
    {
        std::cerr << "IMGUI SFML Window Init failed!" << std::endl;
        return -1;
    }

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::CreateContext();
    ImPlot::CreateContext();

    static MemoryEditor mem_edit;

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                return close_app(window);
            }

            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        // == UPDATE == //
        ImGui::SFML::Update(window, deltaClock.restart());

        if (fr.update_data() == FR_SUCCESS)
        {
            data = fr.get_file_data();
            actual_data_size = data.size();
            calculate_new_ticks();
        }

        // == DRAWING == //
        ImGui::DockSpaceOverViewport();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open", "Ctrl+O"))
                {
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        static ImPlotColormap map = ImPlotColormap_Plasma;
        ImPlot::PushColormap(map);

        if (ImGui::Begin("Plot"))
        {
            if (ImPlot::BeginPlot("Data", ImVec2(-1, -1), ImPlotFlags_NoMouseText | (lock_aspect ? ImPlotFlags_Equal : 0)))
            {
                ImPlotAxisFlags axes_flags = (grid_lines ? 0 : ImPlotAxisFlags_NoGridLines);
                ImPlot::SetupAxes(NULL, NULL, axes_flags, axes_flags);
                ImPlot::SetupAxisTicks(ImAxis_X1, 0.5, cols - 0.5, cols, xs_ticks.data());
                ImPlot::SetupAxisTicks(ImAxis_Y1, 0.5, rows - 0.5, rows, ys_ticks.data());

                if (data.size() > 0)
                {
                    ImPlot::PlotHeatmap("##", data.data(), rows, cols, 0, 255, show_values ? "%d" : "", ImPlotPoint(0, 0), ImPlotPoint(cols, rows), 0);
                }
                ImPlot::EndPlot();
            }
        }
        ImGui::End();

        ImPlot::PopColormap();

        if (data.size() == 0)
        {
            ImGui::BeginDisabled();
        }

        mem_edit.DrawWindow("Hex Viewer", data.data(), data.size());
        if (sync_mem_edit_cols)
        {
            mem_edit.Cols = cols;
        }

        if (ImGui::Begin("Settings"))
        {
            ImGui::Checkbox("Show Values", &show_values);
            ImGui::SameLine();
            ImGui::Checkbox("Grid Lines", &grid_lines);
            ImGui::SameLine();
            ImGui::Checkbox("Keep Square", &lock_aspect);

            ImGui::Separator();

            if (ImGui::InputInt("Columns", &cols))
            {
                if (cols < 1)
                {
                    cols = 1;
                }
                if (cols > (int)actual_data_size)
                {
                    cols = actual_data_size;
                }

                calculate_new_ticks();
            }
            ImGui::Checkbox("Sync Hex Viewer Columns", &sync_mem_edit_cols);
        }
        ImGui::End();

        if (data.size() == 0)
        {
            ImGui::EndDisabled();
        }

        window.clear(sf::Color(20, 20, 20));
        ImGui::SFML::Render(window);
        window.display();
    }

    return close_app(window);
}

int close_app(sf::RenderWindow &window)
{
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    // ImGui::SFML::Shutdown();

    window.close();

    return 0;
}

void calculate_new_ticks()
{
    rows = (actual_data_size + cols - 1) / cols;

    x_ticks.resize(cols);
    y_ticks.resize(rows);
    xs_ticks.resize(cols);
    ys_ticks.resize(rows);

    // FIXME: I don't know how to keep the char* alive otherwise
    for (int c = 0; c < cols; c++)
    {
        x_ticks[c] = (number_to_hex(c));
    }

    for (int16_t r = rows; r > 0; r--)
    {
        y_ticks[rows - r] = (number_to_hex((r - 1) * cols));
    }

    // FIXME: I am pretty sure the double loops are not needed but I can't get it to work otherwise
    for (size_t i = 0; i < x_ticks.size(); i++)
    {
        xs_ticks[i] = x_ticks[i].c_str();
    }

    for (size_t i = 0; i < y_ticks.size(); i++)
    {
        ys_ticks[i] = y_ticks[i].c_str();
    }
    // Fill last row with 0 if there is space left
    if (actual_data_size < (size_t)(rows * cols))
    {
        for (size_t i = 0; i < (rows * cols - actual_data_size); i++)
        {
            data.push_back(0);
        }
    }
}