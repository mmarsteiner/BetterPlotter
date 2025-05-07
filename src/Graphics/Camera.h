#pragma once

#include <cmath>

#include "Mat4.h"
#include "Vec3.h"

namespace g3d {

class Camera {
    double x, y, z, pitch, yaw;
public:
    /**
     * Constructs a Camera with the given parameters. For this system, 
     * there is no need for the camera to be able to "roll," only for 
     * it to look up and down or side to side.
     */
    Camera(double x, double y, double z, double yaw, double pitch);
    void CreateViewMatrix(Mat4& out);
};

}