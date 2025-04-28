//
// Created by mmars on 8/7/2024.
//

#pragma once

#include <Plot/Plot3D/Plot3D.h>
#include <string.h>

#include "Geometry.h"

namespace bp {

void RasterizeLine(const LineGFX& line, uint8_t color);
void RasterizeTri(const TriGFX& tri, uint8_t color);

}  // namespace bp
