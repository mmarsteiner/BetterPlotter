#pragma once

#include <cmath>
#include <cassert>

namespace g3d {

struct Vec4 {
    double x;
    double y;
    double z;
    double w;
    Vec4();
    Vec4(double x, double y, double z, double w);
    double& operator[](int idx);
};

}