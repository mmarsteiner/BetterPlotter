#include "Mat4.h"

namespace g3d {

void Mat4::Transform(const Vec4& in, Vec4& out) const {
    out.x = in.x * data[0] + in.y * data[1] + in.z * data[2] + in.w * data[3];
    out.y = in.x * data[4] + in.y * data[5] + in.z * data[6] + in.w * data[7];
    out.z = in.x * data[8] + in.y * data[9] + in.z * data[10] + in.w * data[11];
    out.w = in.x * data[12] + in.y * data[13] + in.z * data[14] + in.w * data[15];
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

void Mat4::CreateLookAtMatrix(Mat4& out, const Vec3& position, const Vec3& target) {
    Vec3 worldUp {0.0, 0.0, 1.0};
    Vec3 direction, right, up;
    position.Subtract(target, direction);
    direction.UnitVector();
    direction.Cross(worldUp, right);
    right.UnitVector();
    right.Cross(direction, up);
    up.UnitVector();

    Mat4 translation;
    CreateTranslationMatrix(translation, -position.x, -position.y, -position.z);
    Mat4 rotation;
    CreateIdentityMatrix(rotation);
    rotation.data[0] = right.x;     rotation.data[1] = right.y;     rotation.data[2] = right.z;
    rotation.data[4] = up.x;        rotation.data[5] = up.y;        rotation.data[6] = up.z;
    rotation.data[8] = direction.x; rotation.data[9] = direction.y; rotation.data[10] = direction.z;
    rotation.Multiply(translation, out);
}

void Mat4::CreatePerspectiveMatrix(Mat4& out, double near, double far, double aspect, double fov) {
    double invDepth = 1 / (far - near);
    out.data[0] = 1 / (aspect * tan(fov / 2)); out.data[1] = 0;                out.data[2] = 0;                         out.data[3] = 0;                   
    out.data[4] = 0;                           out.data[5] = 1 / tan(fov / 2); out.data[6] = 0;                         out.data[7] = 0;                        
    out.data[8] = 0;                           out.data[9] = 0;                out.data[10] = -(far + near) * invDepth; out.data[11] = -2 * far * near * invDepth;
    out.data[12] = 0;                          out.data[13] = 0;               out.data[14] = -1;                       out.data[15] = 0;                         
}

void Mat4::CreateScreenMatrix(Mat4& out, int width, int height, int x0, int y0) {
    CreateIdentityMatrix(out);
    out.data[0] = width / 2;
    out.data[3] = (width / 2) + x0;
    out.data[5] = -height / 2;
    out.data[7] = (height / 2) + y0;
}

}  // namespace g3d