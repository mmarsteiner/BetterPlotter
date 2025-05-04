//
// Created by mmars on 7/31/2024.
//

#include "QuadtreePlotter.h"

namespace bp {
void QuadtreePlotter::Recurse(const tiparser::AST* func, Plot2D& plot, const Region2D& region, uint32_t depth) {
    if (wasInterrupted) {
        return;
    }
    if (os_GetCSC() == sk_Clear) {
        wasInterrupted = true;
        return;
    }
    if (Settings::GetRect2Settings().GetBool(DEBUG_REGIONS_2)) {
        gfx_SetColor(0xE0);
        plot.OutlineRect(region);
    }
    if (depth < Settings::GetRect2Settings().GetUInt(MIN_REC_2)) {
        Subdivide(func, plot, region, depth);
    } else {
        double funcVals[4];
        GetCornerVals(func, region, funcVals);
        uint8_t code = GetCase(funcVals);
        if (code == 0 || code == 15) {
            return;
        }
        if (depth < Settings::GetRect2Settings().GetUInt(MAX_REC_2)) {
            Subdivide(func, plot, region, depth);
        } else {
            gfx_SetColor(0x18);
            PlotInRegion(funcVals, plot, region);
        }
    }
}

void QuadtreePlotter::Subdivide(const tiparser::AST* func, Plot2D& plot,
                                const Region2D& region, uint32_t depth) {
    Region2D nw = {{region.start.x, region.start.y},
                   region.width / 2.0,
                   region.height / 2.0};
    Recurse(func, plot, nw, depth + 1);
    Region2D ne = {{region.start.x + region.width / 2.0, region.start.y},
                   region.width / 2.0,
                   region.height / 2.0};
    Recurse(func, plot, ne, depth + 1);
    Region2D sw = {{region.start.x, region.start.y - region.height / 2.0},
                   region.width / 2.0,
                   region.height / 2.0};
    Recurse(func, plot, sw, depth + 1);
    Region2D se = {{region.start.x + region.width / 2.0,
                    region.start.y - region.height / 2.0},
                   region.width / 2.0,
                   region.height / 2.0};
    Recurse(func, plot, se, depth + 1);
}

void QuadtreePlotter::PlotInRegion(double* funcVals, Plot2D& plot,
                                   const Region2D& region) {
    Point2D topLeft = {region.start.x, region.start.y};
    Point2D topRight = {region.start.x + region.width, region.start.y};
    Point2D bottomLeft = {region.start.x, region.start.y - region.height};
    Point2D bottomRight = {region.start.x + region.width, region.start.y - region.height};

    uint8_t code = GetCase(funcVals);

    double t1, t2;
    if (code == 1 || code == 14) {
        t1 = GetT(funcVals[0], funcVals[3]);
        t2 = GetT(funcVals[3], funcVals[2]);
        Line2D line = {LinearInterpolate(topLeft, bottomLeft, t1), LinearInterpolate(bottomLeft, bottomRight, t2)};
        plot.DrawLine(line);
    } else if (code == 2 || code == 13) {
        t1 = GetT(funcVals[1], funcVals[2]);
        t2 = GetT(funcVals[3], funcVals[2]);
        Line2D line = {LinearInterpolate(topRight, bottomRight, t1), LinearInterpolate(bottomLeft, bottomRight, t2)};
        plot.DrawLine(line);
    } else if (code == 3 || code == 12) {
        t1 = GetT(funcVals[0], funcVals[3]);
        t2 = GetT(funcVals[1], funcVals[2]);
        Line2D line = {LinearInterpolate(topLeft, bottomLeft, t1), LinearInterpolate(topRight, bottomRight, t2)};
        plot.DrawLine(line);
    } else if (code == 4 || code == 11) {
        t1 = GetT(funcVals[0], funcVals[1]);
        t2 = GetT(funcVals[1], funcVals[2]);
        Line2D line = {LinearInterpolate(topLeft, topRight, t1), LinearInterpolate(topRight, bottomRight, t2)};
        plot.DrawLine(line);
    } else if (code == 5 || code == 10) {
        t1 = GetT(funcVals[0], funcVals[1]);
        t2 = GetT(funcVals[0], funcVals[3]);
        Point2D p0 = LinearInterpolate(topLeft, topRight, t1);
        Point2D p1 = LinearInterpolate(topLeft, bottomLeft, t2);

        t1 = GetT(funcVals[3], funcVals[2]);
        t2 = GetT(funcVals[1], funcVals[2]);
        Point2D p2 = LinearInterpolate(bottomLeft, bottomRight, t1);
        Point2D p3 = LinearInterpolate(topRight, bottomRight, t2);

        if (code == 5) {
            Line2D line1 = {p0, p3};
            Line2D line2 = {p1, p2};
            plot.DrawLine(line1);
            plot.DrawLine(line2);
        } else if (code == 10) {
            Line2D line1 = {p0, p1};
            Line2D line2 = {p2, p3};
            plot.DrawLine(line1);
            plot.DrawLine(line2);
        }
    } else if (code == 6 || code == 9) {
        t1 = GetT(funcVals[0], funcVals[1]);
        t2 = GetT(funcVals[3], funcVals[2]);
        Line2D line = {LinearInterpolate(topLeft, topRight, t1), LinearInterpolate(bottomLeft, bottomRight, t2)};
        plot.DrawLine(line);
    } else if (code == 7 || code == 8) {
        t1 = GetT(funcVals[0], funcVals[1]);
        t2 = GetT(funcVals[0], funcVals[3]);
        Line2D line = {LinearInterpolate(topLeft, topRight, t1), LinearInterpolate(topLeft, bottomLeft, t2)};
        plot.DrawLine(line);
    }
}

void QuadtreePlotter::GetCornerVals(const tiparser::AST* func, const Region2D& region, double* out) {
    uint8_t vars[2] = {OS_TOK_X, OS_TOK_Y};
    double vVals[2] = {region.start.x, region.start.y};
    out[0] = func->Eval(2, vars, vVals);
    vVals[0] += region.width;
    out[1] = func->Eval(2, vars, vVals);
    vVals[1] -= region.height;
    out[2] = func->Eval(2, vars, vVals);
    vVals[0] -= region.width;
    out[3] = func->Eval(2, vars, vVals);
}

uint8_t QuadtreePlotter::GetCase(const double* funcVals) {
    uint8_t code = 0;
    for (int i = 0; i < 4; i++) {
        code = (code << 1) | (funcVals[i] > 0);
    }
    return code;
}

double QuadtreePlotter::GetT(const double f1, const double f2) {
    return fabs(0.0 - f1) / fabs(f2 - f1);
}

Point2D QuadtreePlotter::LinearInterpolate(const Point2D& p0, const Point2D& p1,
                                           const double t) {
    return {(1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y};
}

bool QuadtreePlotter::plotImplicit(const tiparser::AST* func, Plot2D& plot) {
    wasInterrupted = false;
    Region2D startRegion = {{Settings::GetRect2Settings().GetDouble(XMIN_2), Settings::GetRect2Settings().GetDouble(XMAX_2)},
                            Settings::GetRect2Settings().GetDouble(XMAX_2) - Settings::GetRect2Settings().GetDouble(XMIN_2),
                            Settings::GetRect2Settings().GetDouble(YMAX_2) - Settings::GetRect2Settings().GetDouble(YMIN_2)};
    Recurse(func, plot, startRegion, 0);
    return wasInterrupted;
}
}  // namespace bp
