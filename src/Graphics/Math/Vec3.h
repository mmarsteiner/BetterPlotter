#pragma once

#include <cmath>

/**
 * Class for a 3-dimensional, floating point vector.
 */
class Vec3 {
public:
    /** The vector's x component. */
    double x;
    /** The vector's y component. */
    double y;
    /** The vector's z component. */
    double z;

    /** Constructs a vector with a default value of the zero vector. */
    Vec3() : Vec3(0, 0, 0) {}
    /**
     * Constructs a vector with provided x, y, and z components.
     * 
     * @param[in] x The vector's x component.
     * @param[in] y The vector's y component.
     * @param[in] z The vector's z component.
     */
    Vec3(double x, double y, double z) : x{x}, y{y}, z{z} {}

    /**
     * Converts this vector to a unit vector of the same direction in place by dividing
     * each component by the magnitude.
     */
    void UnitVector() {
        double invMag = 1 / Magnitude();
        x *= invMag;
        y *= invMag;
        z *= invMag;
    }

    /**
     * Calculates the length of this vector.
     * 
     * @return length of this vector.
     */
    double Magnitude() const {
        return sqrt(x*x + y*y + z*z);
    }

    /**
     * Scales this vector by a constant factor.
     * 
     * @param[out] out   The scaled vector is placed here.
     * @param[in]  scale The factor to scale the vector by.
     */
    void Scale(Vec3& out, double scale) const {
        out.x = x * scale;
        out.y = y * scale;
        out.z = z * scale;
    }

    /**
     * Calculates the cross product of this vector and another vector.
     * 
     * @param[in]  rhs Vector on the right side of the cross product.
     * @param[out] out The result of `this` "cross" `rhs` is placed here.
     */
    void Cross(const Vec3& rhs, Vec3& out) const {
        out.x = y * rhs.z - z * rhs.y;
        out.y = z * rhs.x - x * rhs.z;
        out.z = x * rhs.y - y * rhs.x;
    }

    /**
     * Calculates the subtraction of this vector and another vector.
     * 
     * @param[in]  rhs Vector on the right side of the subtraction.
     * @param[out] out The result of `this`-`rhs` is placed here.
     */
    void Subtract(const Vec3& rhs, Vec3& out) const {
        out.x = x - rhs.x;
        out.y = y - rhs.y;
        out.z = z - rhs.z;
    }
};