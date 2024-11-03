//
// Created by mmars on 7/30/2024.
//

#include "Plot2D.h"

namespace bp {
PointGFX Plot2D::GetScreenCoords(const Point2D& plotCoords) const {
    int xMargin = (GFX_LCD_WIDTH - GFX_LCD_HEIGHT) / 2;
    PointGFX ret;
    ret.x = xMargin + (plotCoords.x - sm.GetDouble(XMIN_2)) * GFX_LCD_HEIGHT /
                          (sm.GetDouble(XMAX_2) - sm.GetDouble(XMIN_2));
    ret.y = (sm.GetDouble(YMAX_2) - plotCoords.y) * GFX_LCD_HEIGHT /
            (sm.GetDouble(YMAX_2) - sm.GetDouble(YMIN_2));
    return ret;
}

Region2D Plot2D::ConvertRegion(const Region2D& in) const {
    int pixelsPerX =
        GFX_LCD_HEIGHT / (sm.GetDouble(XMAX_2) - sm.GetDouble(XMIN_2));
    int pixelsPerY =
        GFX_LCD_HEIGHT / (sm.GetDouble(YMAX_2) - sm.GetDouble(YMIN_2));
    return {.start = {(in.start.x - sm.GetDouble(XMIN_2)) * pixelsPerX +
                          (GFX_LCD_WIDTH - GFX_LCD_HEIGHT) / 2,
                      (sm.GetDouble(YMAX_2) - in.start.y) * pixelsPerY},
            .width = in.width * pixelsPerX,
            .height = in.height * pixelsPerY};
}

Plot2D::Plot2D(const SettingsManager& sm) : sm{sm} {}

Plot2D::~Plot2D() {}

void Plot2D::Open() {
    os_ClrHomeFull();
    os_DisableCursor();
    gfx_Begin();
    gfx_FillScreen(0xFF);
    gfx_SetColor(0x00);

    if (sm.GetBool(DRAW_GREY_LINES_2)) {
        gfx_SetColor(0x95);
        for (double x = sm.GetDouble(XMIN_2); x <= sm.GetDouble(XMAX_2);
             x += sm.GetDouble(X_TICK_INT_2)) {
            Line2D line(x, sm.GetDouble(YMAX_2), x, sm.GetDouble(YMIN_2));
            DrawLine(line);
        }
        for (double y = sm.GetDouble(YMIN_2); y <= sm.GetDouble(YMAX_2);
             y += sm.GetDouble(Y_TICK_INT_2)) {
            Line2D line(sm.GetDouble(XMIN_2), y, sm.GetDouble(XMAX_2), y);
            DrawLine(line);
        }
        gfx_SetColor(0x00);
    }

    // Draw axes
    Line2D xAxis = {{sm.GetDouble(XMIN_2), 0}, {sm.GetDouble(XMAX_2), 0}};
    Line2D yAxis = {{0, sm.GetDouble(YMIN_2)}, {0, sm.GetDouble(YMAX_2)}};
    DrawLine(xAxis);
    DrawLine(yAxis);

    if (sm.GetBool(DRAW_TICKS_2)) {
        for (double x = sm.GetDouble(XMIN_2); x <= sm.GetDouble(XMAX_2);
             x += sm.GetDouble(X_TICK_INT_2)) {
            Line2D line(x, -0.01, x, 0.01);
            DrawLine(line);
        }
        for (double y = sm.GetDouble(YMIN_2); y <= sm.GetDouble(YMAX_2);
             y += sm.GetDouble(Y_TICK_INT_2)) {
            Line2D line(-0.01, y, 0.01, y);
            DrawLine(line);
        }
    }
}

void Plot2D::Close() {
    gfx_FillScreen(0xFF);
    gfx_End();
}

void Plot2D::DrawLine(const Line2D& line) {
    PointGFX start = GetScreenCoords(line.start);
    PointGFX finish = GetScreenCoords(line.finish);
    gfx_Line(start.x, start.y, finish.x, finish.y);
}

void Plot2D::OutlineRect(const Region2D& region) {
    Region2D converted = ConvertRegion(region);
    gfx_Rectangle(converted.start.x, converted.start.y, converted.width,
                  converted.height);
}
}  // namespace bp
