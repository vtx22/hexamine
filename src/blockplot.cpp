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
   if (_auto_update)
   {

      // Get file last modified time
      std::filesystem::file_time_type ftime = std::filesystem::last_write_time(std::filesystem::path(_path));

      const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(ftime);
      const auto time = std::chrono::system_clock::to_time_t(systemTime);

      if (_last_edit < time)
      {
         if (refresh_file())
         {
            _last_edit = time;
         }
      }
   }

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

   sf::Vector2f highlight_pos;
   uint8_t highlight_value = 0;
   uint64_t hightlight_add = 0;
   bool has_highlight = false;

   for (uint64_t i = 0; i < data.size(); i++)
   {
      sf::Vector2f block_pos(xpos, _main_menu_height + y_offset);
      plot_block(data.at(i), block_pos);

      if (_mouse.x >= block_pos.x && _mouse.x < block_pos.x + _block_size)
      {
         if (_mouse.y >= block_pos.y && _mouse.y < block_pos.y + _block_size)
         {
            highlight_pos = block_pos;
            highlight_value = data.at(i);
            hightlight_add = i;
            has_highlight = true;
         }
      }

      xpos += _block_size;
      block_count++;

      if (block_count >= blocks_per_row)
      {
         xpos = 0;
         block_count = 0;
         y_offset += _block_size;
      }
   }
   if (has_highlight)
   {
      show_highlight(highlight_pos, hightlight_add, highlight_value);
   }
}

void Blockplot::show_highlight(sf::Vector2f block_pos, uint32_t address, uint8_t value)
{
   sf::RectangleShape rect(sf::Vector2f(_block_size, _block_size));
   rect.setPosition(block_pos);

   rect.setFillColor(sf::Color(0, 0, 0, 0));
   float outline = 0.05 * _block_size;
   rect.setOutlineThickness((outline > MIN_HIGHLIGHT_BORDER) ? outline : MIN_HIGHLIGHT_BORDER);
   rect.setOutlineColor(sf::Color(255, 255, 255, 255));
   _window->draw(rect);
}

void Blockplot::set_mouse_pos(sf::Vector2f pos)
{
   _mouse = pos;
}

void Blockplot::set_blocks_per_row(int bpr)
{
   if (bpr < 1)
   {
      bpr = 1;
   }

   _blocks_per_row = bpr;
}

void Blockplot::set_auto_update(bool auto_update)
{
   _auto_update = auto_update;
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