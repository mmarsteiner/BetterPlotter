#include "Rasterizer.h"

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

/* https://w.wiki/87M9 */
void PlotLineLow(int x0, int y0, int x1, int y1, uint8_t color0, uint8_t color1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int D = (2 * dy) - dx;
    int y = y0;
    for (int x = x0; x <= x1; ++x) {
        gfx_vram[y * GFX_LCD_WIDTH + x] = color0;  // TODO: Implement color interpolation and (possibly) depth interpolation
        if (D > 0) {
            y += yi;
            D = D + (2 * (dy - dx));
        } else {
            D = D + 2 * dy;
        }
    }
}

/* https://w.wiki/87M9 */
void PlotLineHigh(int x0, int y0, int x1, int y1, uint8_t color0, uint8_t color1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    int D = (2 * dx) - dy;
    int x = x0;
    for (int y = y0; y <= y1; ++y) {
        gfx_vram[y * GFX_LCD_WIDTH + x] = color0;  // TODO: Implement color interpolation and (possibly) depth interpolation
        if (D > 0) {
            x += xi;
            D = D + (2 * (dx - dy));
        } else {
            D = D + 2 * dx;
        }
    }
}

/**
 * Uses the Bresenham line rasterization algorithm to plot the given line. Source code for `RasterizeLineBresenham`, `PlotLineLow`, and `PlotLineHigh` all come from the Wikipedia page: https://w.wiki/87M9.
 *
 * @param line Line to rasterize
 * @param color0 Pallete color for the first endpoint
 * @param color1 Pallete color for the second endpoint
 */
void RasterizeLineBresenham(const LineGFX& line, uint8_t color0, uint8_t color1) {
    if (abs(line.finish.y - line.start.y) < abs(line.finish.x - line.start.x)) {
        if (line.start.x > line.finish.x) {
            PlotLineLow(line.finish.x, line.finish.y, line.start.x, line.start.y, color0, color1);
        } else {
            PlotLineLow(line.start.x, line.start.y, line.finish.x, line.finish.y, color0, color1);
        }
    } else {
        if (line.start.y > line.finish.y) {
            PlotLineHigh(line.finish.x, line.finish.y, line.start.x, line.start.y, color0, color1);
        } else {
            PlotLineHigh(line.start.x, line.start.y, line.finish.x, line.finish.y, color0, color1);
        }
    }
}
}  // namespace bp
