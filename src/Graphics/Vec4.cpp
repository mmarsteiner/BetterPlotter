#include "Vec4.h"

namespace g3d {

Vec4::Vec4() : Vec4(0, 0, 0, 0) {}

Vec4::Vec4(double x, double y, double z, double w) : x{x}, y{y}, z{z}, w{w} {}

double& Vec4::operator[](int idx) { 
    switch (idx) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
    }
    assert(idx >= 0 && idx < 4); // only for debug mode
    return w;
}

}  // namespace g3d