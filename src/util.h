#ifndef UTIL_HPP
#define UTIL_HPP
#include <sstream>
#include <windows.h>
#include <cstring>

std::string number_to_hex(int num)
{
    std::stringstream ss;
    ss << "" << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << num;
    return ss.str();
}

std::string open_file_explorer(HWND hwnd)
{
    std::string path(MAX_PATH, '\0');
    OPENFILENAME ofn = {sizeof(OPENFILENAME)};
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Binary files (*.bin)\0*.bin\0";
    //"All files\0*.*\0";
    ofn.lpstrFile = &path[0];
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&ofn))
    {
        // string::size() is still MAX_PATH
        // strlen is the actual string size (not including the null-terminator)
        // update size:
        path.resize(strlen(path.c_str()));
    }
    return path;
}

#endif // UTIL_HPP