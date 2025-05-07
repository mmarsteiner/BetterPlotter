//
// Created by mmars on 7/30/2024.
//

#pragma once

#define TEXT_HEIGHT 8
#include <debug.h>

#include <cmath>
#include <cstdint>

namespace bp {
struct PointGFX {
    int x;
    int y;
    uint8_t depth;
    PointGFX() : x(0), y(0), depth(0) {}
    PointGFX(int x, int y) : x{x}, y{y}, depth(0) {}
    PointGFX(int x, int y, uint8_t z) : x{x}, y{y}, depth{z} {}
    void dbg_print(bool newLine) const {
        if (newLine) {
            dbg_printf("(%d, %d)\n", x, y);
        } else {
            dbg_printf("(%d, %d)", x, y);
        }
    }
};

struct Point2D {
    double x;
    double y;
    Point2D(double x, double y) : x{x}, y{y} {}
    Point2D() : Point2D(0, 0) {}
};

struct Region2D {
    Point2D start;  // upper left
    double width;
    double height;
};

struct Line2D {
    Point2D start{0, 0};
    Point2D finish{0, 0};
    Line2D(const Point2D& start, const Point2D& finish)
        : start{start}, finish{finish} {}
    Line2D(double x1, double y1, double x2, double y2) {
        start.x = x1;
        start.y = y1;
        finish.x = x2;
        finish.y = y2;
    }
};

struct LineGFX {
    PointGFX start;
    PointGFX finish;
    LineGFX(const PointGFX& start, const PointGFX& finish)
        : start{start}, finish{finish} {}
};
}  // namespace bp
