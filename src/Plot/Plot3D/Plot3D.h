//
// Created by mmars on 8/1/2024.
//

#pragma once

#include <Parsing/Parser.h>
#include <Plot/Colors.h>
#include <Plot/TriangleRasterizer.h>
#include <graphx.h>

#include "Plot/Geometry.h"
#include "Settings/SettingsManager.h"

#define TRUE_3D_WIDTH (GFX_LCD_HEIGHT - 20)
#define X_MARGIN ((GFX_LCD_WIDTH - TRUE_3D_WIDTH) / 2)
#define Y_MARGIN ((GFX_LCD_HEIGHT - TRUE_3D_WIDTH) / 2)
#define DEG_CONVERSION 0.017532925
#define ROOT_3_OVER_2 0.866025404
#define H 0.001
#define LIGHTING_NORM 0.57735
#define MAX_LF 1.75
#define TEXT_PADDING 2

namespace bp {
typedef enum {
    XMIN_3,
    XMAX_3,
    YMIN_3,
    YMAX_3,
    ZMIN_3,
    ZMAX_3,
    XSTEP_3,
    YSTEP_3,
    ELEVATION_3,
    ROTATION_3,
    DRAW_CONTOUR_LINES,
    SHADE_SURFACE,
    DRAW_OUTER_BOX
} Setting3CID;

typedef enum { THETA_MIN,
               THETA_MAX,
               THETA_STEP,
               ZSTEP } SettingCyl;

typedef enum { TMIN,
               TMAX,
               TSTEP,
               SMIN,
               SMAX,
               SSTEP } SettingPara;

struct MeshDimensions {
    int numRows;
    int numCols;
};

class Plot3D {
    const SettingsManager& sm;

    double sinR{}, cosR{}, sinE{}, cosE{}, pixelsPerX{}, pixelsPerY{},
        xMinFrontal{}, yMinFrontal{}, xMaxFrontal{}, yMaxFrontal{}, p0yMin{},
        p0yMax{}, lightingVecX{}, lightingVecY{},
        lightingVecZ{};  // precalculate

    void GetScreenCoords(const Point3D& plotCoords, PointGFX& output) const;

    double GetFrontalProjection(const Point3D& in, Point2D& output) const;
    void ConvertFrontalToScreen(const Point2D& in, PointGFX& output) const;

    void ConvertPointPerspective(const Point3D& in, PointGFX& output) const;
    PointGFX ConvertPointIsometric(const Point3D& in) const;

    bool AboveZMax(double z) const;
    bool BelowZMin(double z) const;
    bool WithinZBounds(double z) const;

    static Point3D InterpolateByZ(const Point3D& p0, const Point3D& p1,
                                  double zb);
    double GetLightingFactor(const Tri3D& tri) const;
    static uint8_t GetShadeColor(double lightingFactor, BaseColor baseColor);

  public:
    explicit Plot3D(const SettingsManager& sm) : sm{sm} {}
    ~Plot3D() {}

    void Open();
    void Close();

    void AddTri(const Tri3D& tri);
    void AddLine(const Line3D& line);

    void DrawZClippedLine(const Line3D& line, uint8_t color);
    void DrawLine(const Line3D& line, uint8_t color);
    void ShadeTri(const Tri3D& tri, BaseColor color);
};
}  // namespace bp
