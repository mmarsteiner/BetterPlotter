#pragma once

#include <cmath>

#include <Settings/Settings.h>
#include <Settings/SettingsManager.h>
#include <Utils/utils.h>
#include "Math/Vec3.h"
#include "Math/Mat4.h"

#define RADS_PER_DEGREE 0.0174532925
#define VIEW_AREA_SIZE  (GFX_LCD_HEIGHT - 10)
#define ASPECT_RATIO    1
#define FOV             60
#define DISTANCE_FACTOR 1.75
#define RADIANS(x)      RADS_PER_DEGREE * x

#define CUSTOM_BLACK 0
#define CUSTOM_GREY  1
#define CUSTOM_WHITE 2

namespace g3d {

class Renderer {
    /** 
     * Matrix that converts from normalized device coordinates to TI screen coordinates. 
     * Only calculated once because it is independent of settings. 
     */
    Mat4 screenMatrix;
    /** Matrix that converts from 3D world coordinates to TI screen coordinates. */
    Mat4 viewProjectionScreenMatrix;
    /** Bounding box of the plot area. */
    double xMin, xMax, yMin, yMax, zMin, zMax;
    /** The distance from the origin to the furthest corner of the plot cube. */
    double halfPlotDiagonal;
public:
    /**
     * Constructs a renderer using the current system settings.
     */
    Renderer() {
        Mat4::CreateScreenMatrix(screenMatrix, VIEW_AREA_SIZE, VIEW_AREA_SIZE, (GFX_LCD_WIDTH - VIEW_AREA_SIZE) / 2, (GFX_LCD_HEIGHT - VIEW_AREA_SIZE) / 2);

        bp::SettingsManager& settings = bp::Settings::GetRect3Settings();

        double rotation = settings.GetDouble(bp::ROTATION_3);
        double elevation = settings.GetDouble(bp::ELEVATION_3);
        xMin = settings.GetDouble(bp::XMIN_3);
        xMax = settings.GetDouble(bp::XMAX_3);
        yMin = settings.GetDouble(bp::YMIN_3);
        yMax = settings.GetDouble(bp::YMAX_3);
        zMin = settings.GetDouble(bp::ZMIN_3);
        zMax = settings.GetDouble(bp::ZMAX_3);

        double absXMax = fmax(fabs(xMin), fabs(xMax));
        double absYMax = fmax(fabs(yMin), fabs(yMax));
        double absZMax = fmax(fabs(zMin), fabs(zMax));
        halfPlotDiagonal = sqrt(absXMax * absXMax + absYMax * absYMax + absZMax * absZMax);

        UpdateVPSMatrix(rotation, elevation);
    }

    /**
     * Updates `viewProjectionScreenMatrix` with a new rotation and elevation value (potentially 
     * allow user rotation of plots).
     * 
     * @param[in] rotation  Angle between the projection in the xy plane of the Camera's position
     *                      vector and the positive x-axis.
     * @param[in] elevation Angle between the Camera's position vector and the xy plane.
     */
    void UpdateVPSMatrix(double rotation, double elevation) {
        double rotationRads = RADIANS(rotation);
        double elevationRads = RADIANS(elevation);
        Mat4 viewMatrix;
        Vec3 direction{cos(rotationRads), sin(rotationRads), sin(elevationRads)};
        Vec3 position, target;
        direction.Scale(position, halfPlotDiagonal * DISTANCE_FACTOR);
        Mat4::CreateLookAtMatrix(viewMatrix, position, target);

        Mat4 perspectiveMatrix;
        double posMag = position.Magnitude();
        Mat4::CreatePerspectiveMatrix(perspectiveMatrix, posMag - halfPlotDiagonal, posMag + halfPlotDiagonal, ASPECT_RATIO, RADIANS(FOV));

        // Screen matrix already created in constructor because it stays the same regardless of any settings

        Mat4 intermediate;
        screenMatrix.Multiply(perspectiveMatrix, intermediate);
        intermediate.Multiply(viewMatrix, viewProjectionScreenMatrix);
    }

    /**
     * Renders a 3D line from `start` to `end`. Must be called within a graphics context.
     * 
     * @param[in] start First endpoint of the line.
     * @param[in] end   Second endpoint of the line.
     */
    void RenderLine(const Vec4& start, const Vec4& end) const {
        Vec4 screenStart; 
        Vec4 screenEnd;
        viewProjectionScreenMatrix.Transform(start, screenStart);
        viewProjectionScreenMatrix.Transform(end, screenEnd);
        screenStart.Normalize();
        screenEnd.Normalize();
        gfx_Line(screenStart.x, screenStart.y, screenEnd.x, screenEnd.y);
    }

    /**
     * Converts a point from world space to TI screen coordinates.
     * 
     * @param[in]  in  World space coordinate of the point.
     * @param[out] out Location to output the screen coordinate of the point.
     */
    void ConvertPoint(const Vec4& in, Vec4& out) const {
        viewProjectionScreenMatrix.Transform(in, out);
        out.Normalize();
    }

    /**
     * Determines if a point in world space is visible (within bounding box).
     * 
     * @param[in] point Point to check.
     * 
     * @return `true` if `point` is visible or `false` otherwise.
     */
    bool IsVisible(const Vec4& point) const {
        return point.x >= xMin && point.x <= xMax && 
               point.y >= yMin && point.y <= yMax && 
               point.z >= zMin && point.z <= zMax;
    }

    /**
     * Calculates the appropriate color for a given `z` coordinate.
     * 
     * @param[in] z            Z coordinate to find a color for.
     * @param[in] minZAttained Minimum Z coordinate of the function being plotted.
     * @param[in] maxZAttained Maximum Z coordinate of the function being plotted.
     * 
     * @return Palette color that corresponds to the given z coordinate.
     */
    uint8_t GetColor(double z, double minZAttained, double maxZAttained) const {
        if (z <= minZAttained) {
            return 2;
        } else if (z >= maxZAttained) {
            return 255;
        }
        double t = (z - minZAttained) / (maxZAttained - minZAttained);
        return 252 * t + 3; // equivalent to (1-t)*3 + t*255
    }

    /**
     * Initializes the palette for 3D rendering
     */
    void Init3DColors() {
        RGBColor black(0, 0, 0);
        RGBColor grey(130, 130, 130);
        RGBColor white(255, 255, 255);
        gfx_palette[0] = black.To1555();
        gfx_palette[1] = grey.To1555();
        gfx_palette[2] = white.To1555();

        RGBColor toSet(0, 0, 0);
        toSet.FromHSV(0.0, 1.0, 1.0);
        for (int hueDeg = 0; hueDeg < 256 - 3; ++hueDeg) {
            double fHueDeg = static_cast<double>(hueDeg);
            toSet.FromHSV(fHueDeg / 360.0, 1.0, 0.85);
            gfx_palette[hueDeg + 3] = toSet.To1555();
        }
    }

private: // block for color related stuff
    /**
     * Stores an RGB color than can be converted to 1555 format.
     */
    struct RGBColor {
        uint8_t R;
        uint8_t G;
        uint8_t B;
        RGBColor(uint8_t R, uint8_t G, uint8_t B) : R{R}, G{G}, B{B} {}
        uint16_t To1555() const { return gfx_RGBTo1555(R, G, B); }
        void FromHSV(double hue, double s, double v) {
            double r, g, b;
            int i = hue * 6;
            double f = hue * 6 - i;
            double p = v * (1 - s);
            double q = v * (1 - f * s);
            double t = v * (1 - (1 - f) * s);
            switch (i % 6) {
                case 0:
                    r = v; g = t; b = p;
                    break;
                case 1:
                    r = q; g = v; b = p;
                    break;
                case 2:
                    r = p; g = v; b = t;
                    break;
                case 3:
                    r = p; g = q; b = v;
                    break;
                case 4:
                    r = t; g = p; b = v;
                    break;
                case 5:
                    r = v; g = p; b = q;
                    break;
            }
            R = static_cast<uint8_t>(255.0 * r);
            G = static_cast<uint8_t>(255.0 * g);
            B = static_cast<uint8_t>(255.0 * b);
        }
        void DbgPrint() const { dbg_printf("R: %u, G: %u, B: %u", R, G, B); }
    };
    /**
     * Interpolates between two `RGBColor`s
     * 
     * @param[in] a Starting color.
     * @param[in] b End color.
     * @param[in] t Interpolation value between `0` and `1`, where `0` is just `a` and `1` is just `b`.
     * 
     * @return A new `RGBColor` that is `(t*100)`% from `a` to `b`.
     */
    RGBColor InterpolateColor(RGBColor a, RGBColor b, double t) {
        double s = 1 - t;
        return { 
            static_cast<uint8_t>(s * a.R + t * b.R),
            static_cast<uint8_t>(s * a.G + t * b.G),
            static_cast<uint8_t>(s * a.B + t * b.B)
        };
    }
};

}