#pragma once

#include "TriangleRasterizer.h"

namespace bp {

uint8_t min_uint8(uint8_t a, uint8_t b) { return a > b ? b : a; }

int min3(int a, int b, int c) {
    int m = a;
    if (m > b) m = b;
    if (m > c) m = c;
    return m;
}

int max3(int a, int b, int c) {
    int m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    return m;
}

double det(double a, double b, double c, double d) {
    return a * d - b * c;
}

void RasterizeLine(const LineGFX& line, uint8_t color) {
    // dbg_printf("Rasterizing line with color %d\n", color);
    double xDiff = line.finish.x - line.start.x;
    double yDiff = line.finish.y - line.start.y;
    if (xDiff == 0 && yDiff == 0) {
        int xI = line.start.x;
        int yI = line.start.y;
        uint8_t depth = min_uint8(line.start.depth, line.finish.depth);
        if (xI >= 0 && xI < GFX_LCD_WIDTH && yI >= 0 && yI < GFX_LCD_HEIGHT &&
            depth <= gfx_vbuffer[yI][xI]) {
            gfx_vbuffer[yI][xI] = depth;
            gfx_vram[yI * GFX_LCD_WIDTH + xI] = color;
        }
    }
    if (fabs(xDiff) > fabs(yDiff)) {
        double x0, y0, x1, y1, d0, d1;
        if (line.start.x > line.finish.x) {
            x0 = line.finish.x;
            y0 = line.finish.y;
            d0 = line.finish.depth;
            x1 = line.start.x;
            y1 = line.start.y;
            d1 = line.start.depth;
        } else {
            x1 = line.finish.x;
            y1 = line.finish.y;
            d1 = line.finish.depth;
            x0 = line.start.x;
            y0 = line.start.y;
            d0 = line.start.depth;
        }
        double ySlope = (y1 - y0) / (x1 - x0);
        double dSlope = (d1 - d0) / (x1 - x0);
        for (double x = x0; x <= x1; x += 1.0) {
            double y = y0 + (x - x0) * ySlope;
            double d = d0 + (x - x0) * dSlope;
            if (x < 0 || x >= GFX_LCD_WIDTH || y < 0 || y >= GFX_LCD_HEIGHT) {
                continue;
            }
            if (d > 255) {
                d = 255;
            } else if (d < 0) {
                d = 0;
            }
            auto depth = static_cast<uint8_t>(d);
            int xI = static_cast<int>(x);
            int yI = static_cast<int>(y);
            if (depth <= gfx_vbuffer[yI][xI]) {
                gfx_vbuffer[yI][xI] = depth;
                gfx_vram[yI * GFX_LCD_WIDTH + xI] = color;
            }
        }
    } else {
        double x0, y0, x1, y1, d0, d1;
        if (line.start.y > line.finish.y) {
            x0 = line.finish.x;
            y0 = line.finish.y;
            d0 = line.finish.depth;
            x1 = line.start.x;
            y1 = line.start.y;
            d1 = line.start.depth;
        } else {
            x1 = line.finish.x;
            y1 = line.finish.y;
            d1 = line.finish.depth;
            x0 = line.start.x;
            y0 = line.start.y;
            d0 = line.start.depth;
        }
        double xSlope = (x1 - x0) / (y1 - y0);
        double dSlope = (d1 - d0) / (y1 - y0);
        for (double y = y0; y <= y1; y += 1.0) {
            double x = x0 + (y - y0) * xSlope;
            double d = d0 + (y - y0) * dSlope;
            if (x < 0 || x >= GFX_LCD_WIDTH || y < 0 || y >= GFX_LCD_HEIGHT) {
                continue;
            }
            if (d > 255) {
                d = 255;
            } else if (d < 0) {
                d = 0;
            }
            auto depth = static_cast<uint8_t>(d);
            int xI = static_cast<int>(x);
            int yI = static_cast<int>(y);
            if (depth <= gfx_vbuffer[yI][xI]) {
                gfx_vbuffer[yI][xI] = depth;
                gfx_vram[yI * GFX_LCD_WIDTH + xI] = color;
            }
        }
    }
}

void RasterizeTri(const TriGFX& tri, uint8_t color) {
    int minX = min3(tri.p0.x, tri.p1.x, tri.p2.x);
    int maxX = max3(tri.p0.x, tri.p1.x, tri.p2.x);
    int minY = min3(tri.p0.y, tri.p1.y, tri.p2.y);
    int maxY = max3(tri.p0.y, tri.p1.y, tri.p2.y);

    if (minX < 0 || maxX >= GFX_LCD_WIDTH || minY < 0 ||
        maxY >= GFX_LCD_HEIGHT) {
        return;
    }

    // precalculate because these only depend on the triangle, not the pixel
    // being shaded
    double v0x = tri.p1.x - tri.p0.x;
    double v0y = tri.p1.y - tri.p0.y;
    double v1x = tri.p2.x - tri.p0.x;
    double v1y = tri.p2.y - tri.p0.y;
    double dv = tri.p1.depth - tri.p0.depth;
    double dw = tri.p2.depth - tri.p0.depth;
    double denominator = det(v0x, v0y, v1x, v1y);
    if (denominator == 0) {
        denominator = 0.001;
    }
    double invDenom = 1 / denominator;

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            // barycentric coordinates
            double v2x = x - tri.p2.x;
            double v2y = y - tri.p2.y;
            double v3x = x - tri.p0.x;
            double v3y = y - tri.p0.y;
            double v = det(-v1x, v2x, -v1y, v2y) * invDenom;
            double w = det(v0x, v3x, v0y, v3y) * invDenom;
            if (v < 0 || w < 0 || v + w > 1.0) {
                // Point is not in the triangle if any of the barycentric
                // coordinates are negative
                continue;
            }
            uint8_t depth = tri.p0.depth + dv * v + dw * w;
            if (depth < gfx_vbuffer[y][x]) {
                gfx_vbuffer[y][x] = depth;
                gfx_vram[y * GFX_LCD_WIDTH + x] = color;
            }
        }
    }
}
}  // namespace bp
