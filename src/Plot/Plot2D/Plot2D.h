//
// Created by mmars on 7/30/2024.
//

#pragma once

#include <Plot/Geometry.h>
#include <graphx.h>

#include <Settings/Settings.h>

namespace bp {

class Plot2D {
    double xMin{}, xMax{}, yMin{}, yMax{};

    PointGFX GetScreenCoords(const Point2D& plotCoords) const;
    Region2D ConvertRegion(const Region2D& in) const;

    public:
    void Open();
    void Close();

    void DrawLine(const Line2D& line);
    void OutlineRect(const Region2D& region);
};
}  // namespace bp
