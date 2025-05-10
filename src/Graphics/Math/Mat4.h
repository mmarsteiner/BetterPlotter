#pragma once

#include <graphx.h>
#include <debug.h>

#include <cstring>
#include <cmath>

#include "Vec4.h"
#include "Vec3.h"

namespace g3d {

class Mat4 {
    double data[16];

    public:
    void Transform(const Vec4& in, Vec4& out) const;
    void Multiply(const Mat4& rhs, Mat4& out) const;

    static void CreateIdentityMatrix(Mat4& out);
    static void CreateTranslationMatrix(Mat4& out, double tx, double ty, double tz);
    static void CreateLookAtMatrix(Mat4& out, const Vec3& position, const Vec3& target);
    static void CreatePerspectiveMatrix(Mat4& out, double near, double far, double aspect, double fov);
    static void CreateScreenMatrix(Mat4& out, int width, int height, int x0, int y0);
};

}