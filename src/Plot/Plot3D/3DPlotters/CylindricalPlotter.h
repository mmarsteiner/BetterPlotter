//
// Created by mmars on 8/5/2024.
//

#pragma once
#include <Parsing/Parser.h>
#include <Settings/SettingsManager.h>

#include <cmath>

#include "../Plot3D.h"

namespace bp {

class CylindricalPlotter final : public Plotter3D {
    const SettingsManager& smCyl;

  public:
    CylindricalPlotter(const SettingsManager& sm, const SettingsManager& smCyl)
        : Plotter3D(sm), smCyl{smCyl} {}

    bool Plot(const tiparser::AST* func, Plot3D& plot, BaseColor color) const override {
        const char* loadingString = "Loading vertices...";
        gfx_SetColor(0x00);
        gfx_SetDrawScreen();
        gfx_PrintStringXY(
            loadingString,
            GFX_LCD_WIDTH - gfx_GetStringWidth(loadingString) - TEXT_PADDING,
            GFX_LCD_HEIGHT - TEXT_HEIGHT - TEXT_PADDING);
        gfx_SetDrawBuffer();
        Stack<Point3D> points;
        int numRings = 0;
        int numVertices = 0;
        uint8_t vars[2] = {OS_TOK_THETA, OS_TOK_Z};
        double vVals[2] = {0, 0};
        for (double z = sm.GetDouble(ZMIN_3); z <= sm.GetDouble(ZMAX_3);
             z += smCyl.GetDouble(ZSTEP)) {
            if (os_GetCSC() == sk_Clear) {
                while (!points.IsEmpty()) {
                    delete points.Pop();
                }
                return true;
            }
            numRings++;
            numVertices = 0;
            for (double theta = smCyl.GetDouble(THETA_MIN);
                 theta <= smCyl.GetDouble(THETA_MAX);
                 theta += smCyl.GetDouble(THETA_STEP)) {
                vVals[0] = theta;
                vVals[1] = z;
                double r = func->Eval(2, vars, vVals);
                auto* p = new Point3D(r * cos(theta), r * sin(theta), z);
                points.Push(p);
                numVertices++;
            }
        }
        int totalVertices = numRings * numVertices;

        gfx_SetColor(0xFF);
        gfx_SetDrawScreen();
        gfx_FillRectangle(
            GFX_LCD_WIDTH - gfx_GetStringWidth(loadingString) - TEXT_PADDING,
            GFX_LCD_HEIGHT - TEXT_HEIGHT - TEXT_PADDING,
            gfx_GetStringWidth(loadingString), TEXT_HEIGHT);
        gfx_SetColor(0x00);
        const char* renderingString = "Rendering surface...";
        gfx_PrintStringXY(
            renderingString,
            GFX_LCD_WIDTH - gfx_GetStringWidth(renderingString) - TEXT_PADDING,
            GFX_LCD_HEIGHT - TEXT_HEIGHT - TEXT_PADDING);
        gfx_SetDrawBuffer();
        uint8_t contourColor = color;
        if (sm.GetBool(SHADE_SURFACE)) {
            contourColor = 0x00;
        }
        for (size_t i = 0; i < points.Size(); i++) {
            if (os_GetCSC() == sk_Clear) {
                while (!points.IsEmpty()) {
                    delete points.Pop();
                }
                return true;
            }
            Point3D* vertex = points.Get(i);
            if (sm.GetBool(DRAW_CONTOUR_LINES)) {
                gfx_SetColor(0x18);
                if (i % numVertices != 0) {
                    plot.DrawZClippedLine({*vertex, *points.Get(i - 1)},
                                          contourColor);
                }
                if (static_cast<int>(i) - numVertices >= 0) {
                    plot.DrawZClippedLine(
                        {*vertex, *points.Get(i - numVertices)}, contourColor);
                }
            }
            if (sm.GetBool(SHADE_SURFACE)) {
                if (i % numVertices != 0 && i + numVertices < totalVertices) {
                    plot.ShadeTri({*vertex, *points.Get(i - 1),
                                   *points.Get(i + numVertices)},
                                  color);
                }
                if (i % numVertices != numVertices - 1 &&
                    static_cast<int>(i) - numVertices >= 0 &&
                    i + 1 < points.Size()) {
                    plot.ShadeTri({*vertex, *points.Get(i + 1),
                                   *points.Get(i - numVertices)},
                                  color);
                }
            }
        }
        while (!points.IsEmpty()) {
            delete points.Pop();
        }

        gfx_SetColor(0xFF);
        gfx_SetDrawScreen();
        gfx_FillRectangle(
            GFX_LCD_WIDTH - gfx_GetStringWidth(renderingString) - TEXT_PADDING,
            GFX_LCD_HEIGHT - TEXT_HEIGHT - TEXT_PADDING,
            gfx_GetStringWidth(renderingString), TEXT_HEIGHT);
        gfx_SetDrawBuffer();
        gfx_SetColor(0x00);
        return false;
    }
};

}  // namespace bp
