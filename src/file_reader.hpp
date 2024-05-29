#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <fstream>
#include <vector>
#include <cstdint>
#include <chrono>
#include <string>
#include <iostream>
#include <filesystem>

enum file_reader_error
{
    FR_SUCCESS = 0,       // Operation successful
    FR_OPEN_ERROR = -1,   // Could not open file
    FR_READ_ERROR = -2,   // Could not read file
    FR_NOT_MODIFIED = -3, // File has not changed
};

class FileReader
{
public:
    FileReader();
    FileReader(std::string path);

    int update_data();
    int update_data(std::string path);

    const std::vector<uint8_t> &get_file_data() const;

private:
    int read_file();

    std::time_t _last_modified = 0;
    std::vector<uint8_t> _file_data;
    std::string _path;
};

#endif // FILE_READER_HPP