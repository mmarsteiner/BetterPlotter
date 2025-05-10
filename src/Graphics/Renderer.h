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
#define FOV             75
#define RADIANS(x)      RADS_PER_DEGREE * x

namespace g3d {

class Renderer {
    /** 
     * Matrix that converts from normalized device coordinates to TI screen coordinates. 
     * Only calculated once because it is independent of settings. 
     */
    Mat4 screenMatrix;
    /** Matrix that converts from 3D world coordinates to TI screen coordinates. */
    Mat4 viewProjectionScreenMatrix;
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

        double xMax = fmax(fabs(settings.GetDouble(bp::XMIN_3)), fabs(settings.GetDouble(bp::XMAX_3)));
        double yMax = fmax(fabs(settings.GetDouble(bp::YMIN_3)), fabs(settings.GetDouble(bp::YMAX_3)));
        double zMax = fmax(fabs(settings.GetDouble(bp::ZMIN_3)), fabs(settings.GetDouble(bp::ZMAX_3)));
        halfPlotDiagonal = sqrt(xMax * xMax + yMax * yMax + zMax * zMax);

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
        direction.Scale(position, halfPlotDiagonal * 1.1);
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
    void RenderLine(const Vec4& start, const Vec4& end) {
        Vec4 screenStart; 
        Vec4 screenEnd;
        viewProjectionScreenMatrix.Transform(start, screenStart);
        viewProjectionScreenMatrix.Transform(end, screenEnd);
        screenStart.Normalize();
        screenEnd.Normalize();
        gfx_Line(screenStart.x, screenStart.y, screenEnd.x, screenEnd.y);
    }
};

}