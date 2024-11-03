//
// Created by mmars on 7/30/2024.
//

#pragma once

#define TEXT_HEIGHT 8
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

struct Point3D {
    double x;
    double y;
    double z;
    Point3D(double x, double y, double z) : x{x}, y{y}, z{z} {}
};

struct Tri3D {
    Point3D p0{0, 0, 0};
    Point3D p1{0, 0, 0};
    Point3D p2{0, 0, 0};
    Tri3D(const Point3D& p0, const Point3D& p1, const Point3D& p2)
        : p0{p0}, p1{p1}, p2{p2} {}
};

struct Line3D {
    Point3D start{0, 0, 0};
    Point3D finish{0, 0, 0};
    Line3D(const Point3D& start, const Point3D& finish)
        : start{start}, finish{finish} {}
    Line3D(double x1, double y1, double z1, double x2, double y2, double z2) {
        start.x = x1;
        start.y = y1;
        start.z = z1;
        finish.x = x2;
        finish.y = y2;
        finish.z = z2;
    }
    void Translate(const Point3D& vec) {
        start.x += vec.x;
        finish.x += vec.x;
        start.y += vec.y;
        finish.y += vec.y;
        start.z += vec.z;
        finish.z += vec.z;
    }
};

struct LineGFX {
    PointGFX start;
    PointGFX finish;
    LineGFX(const PointGFX& start, const PointGFX& finish)
        : start{start}, finish{finish} {}
    // algorithm from: https://stackoverflow.com/a/1968345
    bool Intersects(const LineGFX& other, PointGFX& output) const {
        PointGFX s1(finish.x - start.x, finish.y - start.y);
        PointGFX s2(other.finish.x - other.start.x,
                    other.finish.y - other.start.y);
        double s = (-s1.y * static_cast<double>(start.x - other.start.x) +
                    s1.x * static_cast<double>(start.y - other.start.y)) /
                   (-s2.x * s1.y + s1.x * s2.y);
        double t = (s2.x * static_cast<double>(start.y - other.start.y) -
                    s2.y * static_cast<double>(start.x - other.start.x)) /
                   (-s2.x * s1.y + s1.x * s2.y);
        if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
            output.x = start.x + t * s1.x;
            output.y = start.y + t * s1.y;
            return true;
        }
        return false;
    }
};

struct TriGFX {
    PointGFX p0;
    PointGFX p1;
    PointGFX p2;
    TriGFX(const PointGFX& p0, const PointGFX& p1, const PointGFX& p2)
        : p0{p0}, p1{p1}, p2{p2} {}
};
}  // namespace bp
