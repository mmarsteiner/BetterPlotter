//
// Created by mmars on 10/5/2024.
//

#pragma once
#include <graphx.h>

#include <cstdint>

namespace bp {
enum BaseColor { 
    BLACK,
    BLUE,
    RED,
    MAGENTA,
    GREEN,
    ORANGE,
    BROWN 
};

struct RGBColor {
    uint8_t R;
    uint8_t G;
    uint8_t B;
    RGBColor(uint8_t R, uint8_t G, uint8_t B) : R{R}, G{G}, B{B} {}
    uint16_t To1555() const;
};

void InitCustomPalette();
}  // namespace bp
