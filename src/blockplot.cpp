#include "blockplot.hpp"

Blockplot::Blockplot(sf::RenderWindow *window) : _window(window)
{
}

std::vector<uint8_t> Blockplot::read_binary_file(std::string path)
{

   if (path.length() == 0)
   {
      return std::vector<uint8_t>(0);
   }

   std::ifstream file(path, std::ios::binary | std::ios::ate);

   if (!file)
   {
      std::cout << "Could not open file " << path << std::endl;
      return std::vector<uint8_t>(0);
   }

   std::streamsize size = file.tellg();
   file.seekg(0, std::ios::beg);

   std::vector<uint8_t> buffer(size);

   if (!file.read(reinterpret_cast<char *>(buffer.data()), size))
   {
      std::cout << "Could not read file " << path << std::endl;
      return std::vector<uint8_t>(0);
   }

   file.close();

   return buffer;
}

bool Blockplot::refresh_file()
{
   std::vector<uint8_t> buf = read_binary_file(_path);

   if (buf.size() == 0)
   {
      return false;
   }

   _bin = buf;

   return true;
}

void Blockplot::update()
{
   plot_blocks_grid(_bin, _blocks_per_row);
}

void Blockplot::plot_block(uint8_t value, sf::Vector2f pos)
{
   sf::RectangleShape shape(sf::Vector2f(_block_size, _block_size));
   shape.setPosition(pos);

   shape.setFillColor(sf::Color(plasma_color[value][0], plasma_color[value][1], plasma_color[value][2]));
   _window->draw(shape);
}

void Blockplot::plot_blocks(const std::vector<uint8_t> &data)
{
   uint32_t xpos = 0;
   for (auto const &b : data)
   {
      plot_block(b, sf::Vector2f(xpos, _main_menu_height));
      xpos += _block_size;
   }
}

void Blockplot::plot_blocks_grid(const std::vector<uint8_t> &data, uint16_t blocks_per_row)
{
   uint16_t block_count = 0;
   uint32_t y_offset = 0;
   uint32_t xpos = 0;

   for (auto const &b : data)
   {
      plot_block(b, sf::Vector2f(xpos, _main_menu_height + y_offset));

      xpos += _block_size;
      block_count++;

      if (block_count >= blocks_per_row)
      {
         xpos = 0;
         block_count = 0;
         y_offset += _block_size;
      }
   }
}

void Blockplot::set_blocks_per_row(int bpr)
{
   if (bpr < 1)
   {
      bpr = 1;
   }

   _blocks_per_row = bpr;
}

void Blockplot::set_zoom(float zoom)
{
   _block_size = MAX_BLOCK_SIZE * zoom;
}

void Blockplot::set_file_path(std::string path)
{
   _path = path;
}

void Blockplot::set_main_menu_height(uint16_t height)
{
   _main_menu_height = height;
}