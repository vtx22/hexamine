#include "file_reader.hpp"

FileReader::FileReader(std::string path) : _path(path)
{
}

FileReader::FileReader()
{
}

int FileReader::read_file()
{
    std::ifstream file(_path, std::ios::binary | std::ios::ate);
    if (!file)
    {
        std::cerr << "Could not open file " << _path << std::endl;
        return FR_OPEN_ERROR;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    _file_data.resize(size, 0);

    if (!file.read(reinterpret_cast<char *>(_file_data.data()), size))
    {
        std::cerr << "Could not read file " << _path << std::endl;
        return FR_READ_ERROR;
    }

    file.close();
    return size;
}

int FileReader::update_data(std::string path)
{
    _path = path;
    return update_data();
}

int FileReader::update_data()
{
    // Get file last modified time
    std::filesystem::file_time_type last_write_f;
    try
    {
        last_write_f = std::filesystem::last_write_time(std::filesystem::path(_path));
    }
    catch (std::filesystem::filesystem_error &e)
    {
        return FR_OPEN_ERROR;
    }

    const auto last_write_st = std::chrono::clock_cast<std::chrono::system_clock>(last_write_f);
    const auto modified_time = std::chrono::system_clock::to_time_t(last_write_st);

    if (_last_modified < modified_time)
    {
        int res = read_file();
        if (res < 0)
        {
            return res;
        }

        _last_modified = modified_time;
        return FR_SUCCESS;
    }

    return FR_NOT_MODIFIED;
}

const std::vector<uint8_t> &FileReader::get_file_data() const
{
    return _file_data;
}