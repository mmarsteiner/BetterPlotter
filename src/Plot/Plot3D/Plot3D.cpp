//
// Created by mmars on 8/1/2024.
//

#include "Plot3D.h"

#include <cmath>

namespace bp {

void Plot3D::GetScreenCoords(const Point3D& plotCoords,
                             PointGFX& output) const {
    ConvertPointPerspective(plotCoords, output);
}

double Plot3D::GetFrontalProjection(const Point3D& in, Point2D& output) const {
    double p1_y = in.x * sinR + in.y * cosR;
    double p0_y = p1_y * cosE - in.z * sinE;
    double pointScaleProj =
        /*Settings::GetRect3Settings().GetDouble(SCREEN_SCALE)*/ 1 *
        (1 / (p0_y - p0yMin + (p0yMax - p0yMin) /*Makes it so that */));
    output.x = pointScaleProj * (in.x * cosR - in.y * sinR);
    output.y = pointScaleProj * (p1_y * sinE + in.z * cosE);
    return p0_y;
}

void Plot3D::ConvertFrontalToScreen(const Point2D& in, PointGFX& output) const {
    output.x = X_MARGIN + pixelsPerX * (in.x - xMinFrontal);
    output.y = Y_MARGIN + pixelsPerY * (yMaxFrontal - in.y);
}

void Plot3D::ConvertPointPerspective(const Point3D& in,
                                     PointGFX& output) const {
    // not technically perspective projection but it works for what this program
    // is
    Point2D frontal(0, 0);
    double p0_y = GetFrontalProjection(
        in, frontal);  // Calculates frontal projection and outputs to frontal
                       // and also calculates depth (p0_y)
    PointGFX ret;
    ConvertFrontalToScreen(frontal, output);
    double d = (p0_y - p0yMin) * (255.0 / (p0yMax - p0yMin));
    output.depth = static_cast<uint8_t>(d);
}

bool Plot3D::AboveZMax(double z) const {
    return z - Settings::GetRect3Settings().GetDouble(ZMAX_3) > 1e-6;
}

bool Plot3D::BelowZMin(double z) const {
    return z - Settings::GetRect3Settings().GetDouble(ZMIN_3) < -1e-6;
}

bool Plot3D::WithinXBounds(double x) const {
    return !(x - Settings::GetRect3Settings().GetDouble(XMAX_3) > 1e-6) && !(x - Settings::GetRect3Settings().GetDouble(XMIN_3) < -1e-6);
}

bool Plot3D::WithinYBounds(double y) const {
    return !(y - Settings::GetRect3Settings().GetDouble(YMAX_3) > 1e-6) && !(y - Settings::GetRect3Settings().GetDouble(YMIN_3) < -1e-6);
}

bool Plot3D::WithinZBounds(double z) const {
    return !AboveZMax(z) && !BelowZMin(z);
}

Point3D Plot3D::InterpolateByZ(const Point3D& p0, const Point3D& p1,
                               double zb) {
    double tb = (zb - p0.z) / (p1.z - p0.z);
    return {p0.x + (p1.x - p0.x) * tb, p0.y + (p1.y - p0.y) * tb, zb};
}

double Plot3D::GetLightingFactor(const Tri3D& tri) const {
    Point3D v1 = {tri.p2.x - tri.p0.x, tri.p2.y - tri.p0.y,
                  tri.p2.z - tri.p0.z};
    Point3D v0 = {tri.p1.x - tri.p0.x, tri.p1.y - tri.p0.y,
                  tri.p1.z - tri.p0.z};
    double cpx = v0.y * v1.z - v0.z * v1.y;
    double cpy = -v0.x * v1.z + v0.z * v1.x;
    double cpz = v0.x * v1.y - v1.x * v0.y;

    // check to make sure normal vector is facing the right direction
    double sign = 1.0;
    double convertedY = (cpx * sinR + cpy * cosR) * cosE - cpz * sinE;
    if (convertedY > 0) {
        sign = -1.0;
    }

    double magnitude = sqrt(cpx * cpx + cpy * cpy + cpz * cpz);
    return sign * ((cpx + cpy + cpz) /
                   magnitude);  // dot product with unit normal vector and <1,
                                // 1, 1>, which points towards light plane
}

uint8_t Plot3D::GetShadeColor(double lightingFactor, BaseColor baseColor) {
    double scaled = 31.0 * (lightingFactor / MAX_LF);
    return static_cast<uint8_t>(baseColor) * 32 + static_cast<uint8_t>(scaled);
}

void Plot3D::Open() {
    double xMin = Settings::GetRect3Settings().GetDouble(XMIN_3);
    double xMax = Settings::GetRect3Settings().GetDouble(XMAX_3);
    double yMin = Settings::GetRect3Settings().GetDouble(YMIN_3);
    double yMax = Settings::GetRect3Settings().GetDouble(YMAX_3);
    double zMin = Settings::GetRect3Settings().GetDouble(ZMIN_3);
    double zMax = Settings::GetRect3Settings().GetDouble(ZMAX_3);
    double R = Settings::GetRect3Settings().GetDouble(ROTATION_3) * DEG_CONVERSION;
    double E = Settings::GetRect3Settings().GetDouble(ELEVATION_3) * DEG_CONVERSION;
    sinR = sin(R);
    cosR = cos(R);
    sinE = sin(E);
    cosE = cos(E);

    double width = xMax - xMin;
    double length = yMax - yMin;
    double height = zMax - zMin;

    os_ClrHomeFull();
    os_DisableCursor();

    gfx_Begin();
    InitCustomPalette();

    // clear screen and non-visible buffer, which is used in this program as the
    // depth buffer
    gfx_FillScreen(0xFF);
    gfx_SetDrawBuffer();
    gfx_FillScreen(0xFF);

    // Find 8 corners of plot and use them to figure out where to place the plot
    // on the screen
    Point3D corners[] = {
        {xMin, yMin, zMin},
        {xMin, yMin, zMax},
        {xMin, yMax, zMin},
        {xMin, yMax, zMax},
        {xMax, yMin, zMin},
        {xMax, yMin, zMax},
        {xMax, yMax, zMin},
        {xMax, yMax, zMax},
    };
    Point2D frontals[8];

    double p1_y_start = xMin * sinR + yMin * cosR;
    double p0_y_start = p1_y_start * cosE - zMin * sinE;
    p0yMin = p0yMax = p0_y_start;
    for (const auto& p : corners) {
        double p1_y = p.x * sinR + p.y * cosR;
        double p0_y = p1_y * cosE - p.z * sinE;
        if (p0_y < p0yMin) {
            p0yMin = p0_y;
        } else if (p0_y > p0yMax) {
            p0yMax = p0_y;
        }
    }

    for (int i = 0; i < 8; i++) {
        GetFrontalProjection(corners[i], frontals[i]);
    }
    xMinFrontal = frontals[0].x;
    yMinFrontal = frontals[0].y;
    xMaxFrontal = frontals[0].x;
    yMaxFrontal = frontals[0].y;
    for (const auto& p : frontals) {
        if (p.x < xMinFrontal) {
            xMinFrontal = p.x;
        } else if (p.x > xMaxFrontal) {
            xMaxFrontal = p.x;
        }
        if (p.y < yMinFrontal) {
            yMinFrontal = p.y;
        } else if (p.y > yMaxFrontal) {
            yMaxFrontal = p.y;
        }
    }

    pixelsPerX = TRUE_3D_WIDTH / (xMaxFrontal - xMinFrontal);
    pixelsPerY = TRUE_3D_WIDTH / (yMaxFrontal - yMinFrontal);

    if (Settings::GetRect3Settings().GetBool(DRAW_OUTER_BOX)) {
        Line3D xLine = {{xMin, yMin, zMin}, {xMax, yMin, zMin}};
        DrawLine(xLine, 0x00);
        xLine.Translate({0, length, 0});
        DrawLine(xLine, 0x00);
        xLine.Translate({0, 0, height});
        DrawLine(xLine, 0x00);
        xLine.Translate({0, -length, 0});
        DrawLine(xLine, 0x00);

        Line3D yLine = {{xMin, yMin, zMin}, {xMin, yMax, zMin}};
        DrawLine(yLine, 0x00);
        yLine.Translate({width, 0, 0});
        DrawLine(yLine, 0x00);
        yLine.Translate({0, 0, height});
        DrawLine(yLine, 0x00);
        yLine.Translate({-width, 0, 0});
        DrawLine(yLine, 0x00);

        Line3D zLine = {{xMin, yMin, zMin}, {xMin, yMin, zMax}};
        DrawLine(zLine, 0x00);
        zLine.Translate({width, 0, 0});
        DrawLine(zLine, 0x00);
        zLine.Translate({0, length, 0});
        DrawLine(zLine, 0x00);
        zLine.Translate({-width, 0, 0});
        DrawLine(zLine, 0x00);
    }

    const Line3D xAxis = {{xMin, 0, 0}, {xMax, 0, 0}};
    const Line3D yAxis = {{0, yMin, 0}, {0, yMax, 0}};
    const Line3D zAxis = {{0, 0, zMin}, {0, 0, zMax}};
    DrawLine(xAxis, 0x00);
    DrawLine(yAxis, 0x00);
    DrawLine(zAxis, 0x00);
    gfx_SetDrawScreen();

    Point3D xLabelLoc = {xMax + 0.05 * width, 0, 0};
    PointGFX xLabelScreenLoc;
    GetScreenCoords(xLabelLoc, xLabelScreenLoc);
    gfx_PrintStringXY("X", xLabelScreenLoc.x - gfx_GetStringWidth("X") / 2, xLabelScreenLoc.y - TEXT_HEIGHT / 2);
    gfx_SetDrawBuffer();
    gfx_SetColor(xLabelScreenLoc.depth);
    gfx_PrintStringXY("X", xLabelScreenLoc.x - gfx_GetStringWidth("X") / 2, xLabelScreenLoc.y - TEXT_HEIGHT / 2);
    gfx_SetColor(0x00);
    gfx_SetDrawScreen();

    Point3D yLabelLoc = {0, yMax + 0.05 * length, 0};
    PointGFX yLabelScreenLoc;
    GetScreenCoords(yLabelLoc, yLabelScreenLoc);
    gfx_PrintStringXY("Y", yLabelScreenLoc.x - gfx_GetStringWidth("Y") / 2, yLabelScreenLoc.y - TEXT_HEIGHT / 2);
    gfx_SetDrawBuffer();
    gfx_SetColor(yLabelScreenLoc.depth);
    gfx_PrintStringXY("Y", yLabelScreenLoc.x - gfx_GetStringWidth("Y") / 2, yLabelScreenLoc.y - TEXT_HEIGHT / 2);
    gfx_SetColor(0x00);
    gfx_SetDrawScreen();

    Point3D zLabelLoc = {0, 0, zMax + 0.05 * height};
    PointGFX zLabelScreenLoc;
    GetScreenCoords(zLabelLoc, zLabelScreenLoc);
    gfx_PrintStringXY("Z", zLabelScreenLoc.x - gfx_GetStringWidth("Z") / 2, zLabelScreenLoc.y - TEXT_HEIGHT / 2);
    gfx_SetDrawBuffer();
    gfx_SetColor(zLabelScreenLoc.depth);
    gfx_PrintStringXY("Z", zLabelScreenLoc.x - gfx_GetStringWidth("Z") / 2, zLabelScreenLoc.y - TEXT_HEIGHT / 2);
    gfx_SetColor(0x00);
    gfx_SetDrawScreen();

    gfx_SetDrawBuffer();
}

void Plot3D::Close() {
    gfx_FillScreen(0xFF);
    gfx_End();
}

void Plot3D::DrawZClippedLine(const Line3D& line, uint8_t color) {
    const Point3D& p0 = line.start;
    const Point3D& p1 = line.finish;
    // both points inside
    if (WithinZBounds(p0.z) && WithinZBounds(p1.z)) {
        DrawLine(line, color);
    }
    // both points out of clipping zone but line does not intersect the clipping
    // zone
    if ((AboveZMax(p0.z) && AboveZMax(p1.z)) ||
        (BelowZMin(p0.z) && BelowZMin(p1.z))) {
        return;
    }
    // line crosses clipping zone but both endpoints are outside the zone
    if (!WithinZBounds(p0.z) && !WithinZBounds(p1.z)) {
        Point3D newP0 = InterpolateByZ(p0, p1, Settings::GetRect3Settings().GetDouble(ZMAX_3));
        Point3D newP1 = InterpolateByZ(p0, p1, Settings::GetRect3Settings().GetDouble(ZMIN_3));
        DrawLine({newP0, newP1}, color);
        return;
    }
    // one inside and one above
    if (AboveZMax(p0.z) && WithinZBounds(p1.z)) {
        Point3D newP0 = InterpolateByZ(p0, p1, Settings::GetRect3Settings().GetDouble(ZMAX_3));
        DrawLine({newP0, p1}, color);
        return;
    }
    if (AboveZMax(p1.z) && WithinZBounds(p0.z)) {
        Point3D newP1 = InterpolateByZ(p0, p1, Settings::GetRect3Settings().GetDouble(ZMAX_3));
        DrawLine({p0, newP1}, color);
        return;
    }
    // one inside and one below
    if (BelowZMin(p0.z) && WithinZBounds(p1.z)) {
        Point3D newP0 = InterpolateByZ(p0, p1, Settings::GetRect3Settings().GetDouble(ZMIN_3));
        DrawLine({newP0, p1}, color);
        return;
    }
    if (BelowZMin(p1.z) && WithinZBounds(p0.z)) {
        Point3D newP1 = InterpolateByZ(p0, p1, Settings::GetRect3Settings().GetDouble(ZMIN_3));
        DrawLine({p0, newP1}, color);
        return;
    }
}

void Plot3D::DrawLine(const Line3D& line, uint8_t color) {
    PointGFX start, finish;
    GetScreenCoords(line.start, start);
    GetScreenCoords(line.finish, finish);
    RasterizeLine({start, finish}, color * 32);
}

void Plot3D::ShadeTri(const Tri3D& tri, BaseColor baseColor) {
    if (!WithinZBounds(tri.p0.z) || !WithinZBounds(tri.p1.z) ||
        !WithinZBounds(tri.p2.z) || !WithinXBounds(tri.p0.x) ||
        !WithinXBounds(tri.p1.x) || !WithinXBounds(tri.p2.x) ||
        !WithinYBounds(tri.p0.y) || !WithinYBounds(tri.p1.y) ||
        !WithinYBounds(tri.p2.y)) {
        return;
    }
    double lf = GetLightingFactor(tri);
    if (lf < 0) {
        lf = 0;
    }
    PointGFX p0, p1, p2;
    GetScreenCoords(tri.p0, p0);
    GetScreenCoords(tri.p1, p1);
    GetScreenCoords(tri.p2, p2);
    RasterizeTri({p0, p1, p2}, GetShadeColor(lf, baseColor));
}
}  // namespace bp
