#pragma once

#include <string>

namespace Color {

    uint32_t Parse(const std::string str);
    std::string String(uint32_t color);

} // namespace Color
