//
// Created by mmars on 7/30/2024.
//

#include "Plot2D.h"

namespace bp {
PointGFX Plot2D::GetScreenCoords(const Point2D& plotCoords) const {
    int xMargin = (GFX_LCD_WIDTH - GFX_LCD_HEIGHT) / 2;
    PointGFX ret;
    ret.x = xMargin + (plotCoords.x - xMin) * GFX_LCD_HEIGHT / (xMax - xMin);
    ret.y = (yMax - plotCoords.y) * GFX_LCD_HEIGHT / (yMax - yMin);
    return ret;
}

Region2D Plot2D::ConvertRegion(const Region2D& in) const {
    int pixelsPerX = GFX_LCD_HEIGHT / (xMax - xMin);
    int pixelsPerY = GFX_LCD_HEIGHT / (yMax - yMin);
    return {
        .start = {
            (in.start.x - xMin) * pixelsPerX + (GFX_LCD_WIDTH - GFX_LCD_HEIGHT) / 2, 
            (yMax - in.start.y) * pixelsPerY
        },
        .width = in.width * pixelsPerX,
        .height = in.height * pixelsPerY};
}

void Plot2D::Open() {
    xMin = Settings::GetRect2Settings().GetDouble(XMIN_2);
    xMax = Settings::GetRect2Settings().GetDouble(XMAX_2);
    yMin = Settings::GetRect2Settings().GetDouble(YMIN_2);
    yMax = Settings::GetRect2Settings().GetDouble(YMAX_2);
    double xInt = Settings::GetRect2Settings().GetDouble(X_TICK_INT_2);
    double yInt = Settings::GetRect2Settings().GetDouble(Y_TICK_INT_2);

    os_ClrHomeFull();
    os_DisableCursor();
    gfx_Begin();
    gfx_FillScreen(0xFF);
    gfx_SetColor(0x00);

    if (Settings::GetRect2Settings().GetBool(DRAW_GREY_LINES_2)) {
        gfx_SetColor(0x95);
        for (double x = xMin; x <= xMax; x += xInt) {
            Line2D line(x, yMax, x, yMin);
            DrawLine(line);
        }
        for (double y = yMin; y <= yMax; y += yInt) {
            Line2D line(xMin, y, xMax, y);
            DrawLine(line);
        }
        gfx_SetColor(0x00);
    }

    // Draw axes
    Line2D xAxis = {{xMin, 0}, {xMax, 0}};
    Line2D yAxis = {{0, yMin}, {0, yMax}};
    DrawLine(xAxis);
    DrawLine(yAxis);

    if (Settings::GetRect2Settings().GetBool(DRAW_TICKS_2)) {
        for (double x = xMin; x <= xMax; x += xInt) {
            Line2D line(x, -0.01, x, 0.01);
            DrawLine(line);
        }
        for (double y = yMin; y <= yMax; y += yInt) {
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
    gfx_Rectangle(converted.start.x, converted.start.y, converted.width, converted.height);
}
}  // namespace bp
