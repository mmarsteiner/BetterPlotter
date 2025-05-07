#pragma once

#include <cmath>
#include <cassert>

namespace g3d {

class Vec4 {
    double data[4] {};
public:
    double& x;
    double& y;
    double& z;
    double& w;

    Vec4() : Vec4(0, 0, 0, 0) {}
    Vec4(double x, double y, double z, double w) : x{data[0]}, y{data[1]}, z{data[2]}, w{data[3]} {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = w;
    }

    double& operator[](int idx) { 
        return data[idx];
    }
};

}