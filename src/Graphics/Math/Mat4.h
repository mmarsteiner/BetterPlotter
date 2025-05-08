#pragma once

#include <graphx.h>

#include <cstring>
#include <cmath>

#include "Vec4.h"

namespace g3d {

class Mat4 {
    double data[16];

    public:
    void Transform(const Vec4& in, Vec4& out) const;
    void Multiply(const Mat4& rhs, Mat4& out) const;

    static void CreateIdentityMatrix(Mat4& out);
    static void CreateTranslationMatrix(Mat4& out, double tx, double ty, double tz);
    static void CreateRotationMatrixX(Mat4& out, double theta);
    static void CreateRotationMatrixY(Mat4& out, double theta);
    static void CreateRotationMatrixZ(Mat4& out, double theta);
    static void CreatePerspectiveMatrix(Mat4& out, double near, double far, double right, double left, double top, double bottom);
    static void CreateScreenMatrix(Mat4& out, int width, int height, int x0, int y0);
};

}