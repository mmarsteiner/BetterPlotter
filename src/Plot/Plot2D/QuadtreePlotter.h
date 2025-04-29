//
// Created by mmars on 7/31/2024.
//

#pragma once

#include "Parsing/Parser.h"
#include "Plot/Geometry.h"
#include "Plot2D.h"

namespace bp {
class QuadtreePlotter {
    bool wasInterrupted;
    const SettingsManager& sm;

    void Recurse(const tiparser::AST* func, Plot2D& plot, const Region2D& region, uint32_t depth);
    void Subdivide(const tiparser::AST* func, Plot2D& plot, const Region2D& region, uint32_t depth);

    static void PlotInRegion(double* funcVals, Plot2D& plot, const Region2D& region);

    static void GetCornerVals(const tiparser::AST* func, const Region2D& region, double* out);
    static uint8_t GetCase(const double* funcVals);
    static double GetT(double f1, double f2);
    static Point2D LinearInterpolate(const Point2D& p0, const Point2D& p1, double t);

    public:
    explicit QuadtreePlotter(const SettingsManager& sm);
    bool plotImplicit(const tiparser::AST* func, Plot2D& plot);
};
}  // namespace bp
