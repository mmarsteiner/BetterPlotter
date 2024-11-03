//
// Created by mmars on 7/30/2024.
//

#pragma once

#include <Plot/Geometry.h>
#include <graphx.h>

#include "Settings/SettingsManager.h"

namespace bp {
typedef enum {
    XMIN_2,
    XMAX_2,
    YMIN_2,
    YMAX_2,
    DRAW_TICKS_2,
    X_TICK_INT_2,
    Y_TICK_INT_2,
    DRAW_GREY_LINES_2,
    MIN_REC_2,
    MAX_REC_2,
    DEBUG_REGIONS_2
} Setting2ID;

class Plot2D {
    const SettingsManager& sm;
    PointGFX GetScreenCoords(const Point2D& plotCoords) const;
    Region2D ConvertRegion(const Region2D& in) const;

   public:
    explicit Plot2D(const SettingsManager& sm);
    ~Plot2D();

    void Open();
    void Close();

    void DrawLine(const Line2D& line);
    void OutlineRect(const Region2D& region);
};
}  // namespace bp
