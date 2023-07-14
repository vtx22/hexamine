#ifndef BLOCKPLOT_HPP
#define BLOCKPLOT_HPP

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>
#include <iostream>

#include "color_lookup.h"

#include <SFML/Graphics.hpp>

#define MAX_BLOCK_SIZE 100

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

   void set_zoom(float zoom);
   void set_blocks_per_row(int bpr);
   void set_file_path(std::string path);
   void set_main_menu_height(uint16_t height);

private:
   std::vector<uint8_t> _bin;

   bool _update = true;

   uint16_t _blocks_per_row = 1;
   uint16_t _main_menu_height = 0;

   uint8_t _block_size = 10;

   std::string _path;
   sf::RenderWindow *_window;
};

#endif // BLOCKPLOT_HPP