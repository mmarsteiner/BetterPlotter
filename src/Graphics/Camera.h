#pragma once

#include <cmath>

#include "Math/Mat4.h"

namespace g3d {

class Camera {
    double x, y, z, pitch, yaw;
public:
    /**
     * Constructs a Camera with the given parameters. For this system, 
     * there is no need for the camera to be able to "roll," only for 
     * it to look up and down or side to side.
     * 
     * @param x     X coordinate of the camera
     * @param y     Y coordinate of the camera
     * @param z     Z coordinate of the camera
     * @param yaw   yaw of the camera (rotation around its vertical 
     *              Y axis), applied second
     * @param pitch pitch of the camera (rotation around its horizontal  
     *              X axis), applied first
     */
    Camera(double x, double y, double z, double yaw, double pitch);
    /**
     * Creates the matrix that transforms from world space to view space. 
     * This matrix transforms the camera from its location to be at the 
     * origin and pointing in the -z direction.
     * 
     * @param out Reference to the output location. The constructed view 
     *            matrix will be placed here.
     */
    void CreateViewMatrix(Mat4& out);
};

}