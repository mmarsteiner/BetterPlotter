//
// Created by mmars on 10/5/2024.
//

#include "Colors.h"

// t is between zero and 1, t=0 is a and t=1 is b
uint8_t Interpolate(uint8_t a, uint8_t b, double t);
bp::RGBColor InterpolateColor(const bp::RGBColor& a, const bp::RGBColor& b, double t) {
    return {
        Interpolate(a.R, b.R, t), Interpolate(a.G, b.G, t),
        Interpolate(a.B, b.B, t)
    };
}

uint16_t bp::RGBColor::To1555() const { return gfx_RGBTo1555(R, G, B); }

void bp::InitCustomPalette() {
    RGBColor startingColors[] = {
        RGBColor(0, 0, 0), RGBColor(0, 0, 255),  // blue
        RGBColor(255, 0, 0),                     // red
        RGBColor(150, 0, 140),                   // magenta
        RGBColor(0, 150, 0),                     // green
        RGBColor(200, 130, 0),                   // orange
        RGBColor(90, 70, 40),                    // brown
    };
    RGBColor white(255, 255, 255);
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 32; col++) {
            double t = col / 32.0;
            gfx_palette[row * 32 + col] = InterpolateColor(startingColors[row], white, t).To1555();
        }
    }
}

uint8_t Interpolate(uint8_t a, uint8_t b, double t) {
    return static_cast<uint8_t>(static_cast<double>(a) * (1.0 - t) + static_cast<double>(b) * t);
}
