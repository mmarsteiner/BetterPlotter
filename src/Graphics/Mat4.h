#pragma once

#include <cstring>
#include "Vec4.h"

namespace g3d {

class Mat4 {
    double data[16];

    public:
    void Transform(const Vec4& in, Vec4& out) const;
    void Multiply(const Mat4& rhs, Mat4& out) const;
};

}