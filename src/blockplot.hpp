#ifndef BLOCKPLOT_HPP
#define BLOCKPLOT_HPP

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <filesystem>
#include <chrono>

#include "color_lookup.h"

#include <SFML/Graphics.hpp>

using namespace std::chrono_literals;

#define MAX_BLOCK_SIZE 100
#define MIN_HIGHLIGHT_BORDER 3

class Blockplot
{
public:
   Blockplot(sf::RenderWindow *window);

   bool refresh_file();
   static std::vector<uint8_t> read_binary_file(std::string path);

   void plot_block(uint8_t value, sf::Vector2f pos);
   void plot_blocks(const std::vector<uint8_t> &data);
   void plot_blocks_grid(const std::vector<uint8_t> &data, uint16_t blocks_per_row);

   void update();

   static std::string get_ascii_string(uint8_t c);

   void set_hover_highlight(bool hover_highlight);
   void set_auto_update(bool auto_update);
   void set_zoom(float zoom);
   void set_blocks_per_row(int bpr);
   void set_block_offset(uint32_t offset);
   void set_file_path(std::string path);
   void set_main_menu_height(uint16_t height);
   void set_mouse_pos(sf::Vector2f pos);

   uint64_t get_total_size();

   uint8_t get_selected_value();
   uint64_t get_selected_address();
   uint64_t get_last_edit();

private:
   std::vector<uint8_t> _bin;

   sf::Vector2f calculate_highlight_block(sf::Vector2f mouse_pos);
   void show_highlight(sf::Vector2f block_pos, uint32_t address, uint8_t value);

   bool _update = true;
   bool _auto_update = false;
   bool _hover_highlight = true;

   uint16_t _blocks_per_row = 32;
   uint32_t _block_offset = 0;
   uint16_t _main_menu_height = 0;

   uint8_t _selected_val;
   uint64_t _selected_add;

   uint8_t _block_size = MAX_BLOCK_SIZE * 0.3;

   std::time_t _last_edit = 0;

   sf::Vector2f _mouse;
   sf::Font _font;

   std::string _path;
   sf::RenderWindow *_window;
};

#endif // BLOCKPLOT_HPP