#pragma once

#include <cmath>
#include <cassert>
#include <debug.h>

namespace g3d {

struct Vec4 {
    double x, y, z, w;
    Vec4() : Vec4(0, 0, 0, 0) {}
    Vec4(double x, double y, double z, double w) : x{x}, y{y}, z{z}, w{w} {}
    void Normalize() {
        double invDenom = 1 / w;
        x *= invDenom;
        y *= invDenom;
        z *= invDenom;
        w = 1.0;
    }
};

}