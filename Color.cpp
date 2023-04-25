#include "Color.h"
#include <regex>

uint32_t Color::parse(const std::string str) {
    std::string s = str;
    std::smatch m;
    if (std::regex_match(s, m, std::regex("^\\s*#([0-9a-zA-Z]{3})\\s*$"))) {
        long rgb = std::stol(m[1].str(), nullptr, 16);
        long r = (rgb >> 8) & 0x0f;
        long g = (rgb >> 4) & 0x0f;
        long b = rgb & 0x0f;
        return static_cast<uint32_t>( (((r << 4) | r) << 16) | (((g << 4) | g) << 8) | ((b << 4) | b) );
    }
    else if (std::regex_match(s, m, std::regex("^\\s*#([0-9a-zA-Z]{6})\\s*$"))) {
        return static_cast<uint32_t>(std::stol(m[1].str(), nullptr, 16));
    }
    return 0;
}
