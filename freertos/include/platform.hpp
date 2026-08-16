#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <stdint.h>

namespace platform
{
struct RgbColor
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

void initialize();
void writeWs2812(RgbColor color);
}

#endif
