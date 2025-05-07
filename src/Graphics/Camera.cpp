#include "Camera.h"

namespace g3d {

Camera::Camera(double x, double y, double z, double pitch, double yaw) 
    : x{x}, y{y}, z{z}, pitch{pitch}, yaw{yaw} {}

void Camera::CreateViewMatrix(Mat4& out) {
    // translate camera back to (0, 0)
    Mat4 antiTranslation;
    Mat4::CreateTranslationMatrix(antiTranslation, -x, -y, -z);
    // un-pitch camera
    Mat4 antiPitch;
    Mat4::CreateRotationMatrixX(antiPitch, -pitch);
    // un-yaw camera
    Mat4 antiYaw;
    Mat4::CreateRotationMatrixY(antiYaw, -yaw);
    // final = antiYaw * antiPitch * antiTranslation
    Mat4 antiRotation;
    antiYaw.Multiply(antiPitch, antiRotation);
    antiRotation.Multiply(antiTranslation, out);
}

}