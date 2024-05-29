#ifndef UTIL_HPP
#define UTIL_HPP
#include <sstream>

std::string number_to_hex(int num)
{
    std::stringstream ss;
    ss << "0x" << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << num;
    return ss.str();
}

#endif // UTIL_HPP