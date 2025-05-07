#include "Mat4.h"

namespace g3d {

void Mat4::Transform(const Vec4& in, Vec4& out) const {
    for (int i = 0; i < 16; i += 4) {
        out[i] = in.x * data[i] + in.y * data[i + 1] + in.z * data[i + 2] + in.w * data[i + 3];
    }
}

void Mat4::Multiply(const Mat4& rhs, Mat4& out) const {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            double sum = 0.0;
            for (int i = 0; i < 4; ++i) {
                sum += data[row * 4 + i] * rhs.data[i * 4 + col];
            }
            out.data[row * 4 + col] = sum;
        }
    }
}

void Mat4::CreateIdentityMatrix(Mat4& out) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (row == col) {
                out.data[row * 4 + col] = 1;
            } else {
                out.data[row * 4 + col] = 0;
            }
        }
    }
}

void Mat4::CreateTranslationMatrix(Mat4& out, double tx, double ty, double tz) {
    CreateIdentityMatrix(out);
    out.data[3] = tx;
    out.data[7] = ty;
    out.data[11] = tz;
}

void Mat4::CreateRotationMatrixX(Mat4& out, double theta) {
    CreateIdentityMatrix(out);
    out.data[5] = cos(theta);
    out.data[6] = -sin(theta);
    out.data[9] = sin(theta);
    out.data[10] = cos(theta);
}

void Mat4::CreateRotationMatrixY(Mat4& out, double theta) {
    CreateIdentityMatrix(out);
    out.data[0] = cos(theta);
    out.data[2] = sin(theta);
    out.data[8] = -sin(theta);
    out.data[10] = cos(theta);
}

void Mat4::CreateRotationMatrixZ(Mat4& out, double theta) {
    CreateIdentityMatrix(out);
    out.data[0] = cos(theta);
    out.data[1] = -sin(theta);
    out.data[4] = sin(theta);
    out.data[5] = cos(theta);
}

void Mat4::CreatePerspectiveMatrix(Mat4& out, double near, double far, double right, double left, double top, double bottom) {
    double invWidth = 1 / (right - left);
    double invHeight = 1 / (top - bottom);
    double invDepth = 1 / (far - near);
    out.data[0] = 2 * near * invWidth;         // [0][0]
    out.data[1] = 0;                           // [0][1]
    out.data[2] = (right + left) * invWidth;   // [0][2]
    out.data[3] = 0;                           // [0][3]
    out.data[4] = 0;                           // [1][0]
    out.data[5] = 2 * near * invHeight;        // [1][1]
    out.data[6] = (top + bottom) * invHeight;  // [1][2]
    out.data[7] = 0;                           // [1][3]
    out.data[8] = 0;                           // [2][0]
    out.data[9] = 0;                           // [2][1]
    out.data[10] = -(far + near) * invDepth;   // [2][2]
    out.data[11] = -2 * far * near * invDepth; // [2][3]
    out.data[12] = 0;                          // [3][0]
    out.data[13] = 0;                          // [3][1]
    out.data[14] = -1;                         // [3][2]
    out.data[15] = 0;                          // [3][3]
}

}  // namespace g3d