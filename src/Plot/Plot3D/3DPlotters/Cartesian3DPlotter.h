#pragma once

#include <Parsing/Parser.h>
#include <Plot/Plot3D/Plot3D.h>
#include <Plot/Plot3D/Plotter3D.h>
#include <Settings/SettingsManager.h>
#include <graphx.h>
#include <ti/tokens.h>

namespace bp {

class Cartesian3DPlotter final : public Plotter3D {
  public:
    explicit Cartesian3DPlotter(const SettingsManager& sm) : Plotter3D(sm) {}
    bool Plot(const tiparser::AST* func, Plot3D& plot, BaseColor color) const override {
        gfx_SetDrawScreen();
        const char* loadingString = "Loading vertices...";
        gfx_SetColor(0x00);
        gfx_PrintStringXY(
            loadingString,
            GFX_LCD_WIDTH - gfx_GetStringWidth(loadingString) - TEXT_PADDING,
            GFX_LCD_HEIGHT - TEXT_HEIGHT - TEXT_PADDING);
        gfx_SetDrawBuffer();

        Stack<Point3D> points;
        int numRows = 0;
        int numCols = 0;
        uint8_t vars[2] = {OS_TOK_X, OS_TOK_Y};
        double vVals[2];
        for (double x = sm.GetDouble(XMIN_3); x <= sm.GetDouble(XMAX_3);
             x += sm.GetDouble(XSTEP_3)) {
            if (os_GetCSC() == sk_Clear) {
                while (!points.IsEmpty()) {
                    delete points.Pop();
                }
                return true;
            }
            numRows++;
            numCols = 0;
            for (double y = sm.GetDouble(YMIN_3); y <= sm.GetDouble(YMAX_3);
                 y += sm.GetDouble(YSTEP_3)) {
                vVals[0] = x;
                vVals[1] = y;
                auto* p = new Point3D(x, y, func->Eval(2, vars, vVals));
                points.Push(p);
                numCols++;
            }
        }
        int meshSize = numRows * numCols;

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
            Point3D* corner = points.Get(i);
            if (sm.GetBool(DRAW_CONTOUR_LINES)) {
                if (i % numCols != 0) {
                    plot.DrawZClippedLine({*corner, *points.Get(i - 1)},
                                          contourColor);
                }
                if (static_cast<int>(i) - numCols >= 0) {
                    plot.DrawZClippedLine({*corner, *points.Get(i - numCols)},
                                          contourColor);
                }
            }
            if (sm.GetBool(SHADE_SURFACE)) {
                if (i % numCols != 0 && i + numCols < meshSize) {
                    plot.ShadeTri(
                        {*corner, *points.Get(i - 1), *points.Get(i + numCols)},
                        color);
                }
                if (i % numCols != numCols - 1 &&
                    static_cast<int>(i) - numCols >= 0) {
                    plot.ShadeTri(
                        {*corner, *points.Get(i + 1), *points.Get(i - numCols)},
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
