#include "Vec3.h"

namespace g3d {

Vec3::Vec3() : Vec3(0, 0, 0) {}

Vec3::Vec3(double x, double y, double z) : x{x}, y{y}, z{z} {}

double& Vec3::operator[](int idx) { 
    switch (idx) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
    }
    assert(idx >= 0 && idx < 3); // only for debug mode
    return z;
}

}  // namespace g3d