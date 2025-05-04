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

}  // namespace g3d