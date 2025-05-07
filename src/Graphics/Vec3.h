#pragma once

#include <cmath>
#include <cassert>

namespace g3d {

struct Vec3 {
    double x;
    double y;
    double z;
    Vec3();
    Vec3(double x, double y, double z);
    double& operator[](int idx);
};

}