#pragma once

#include <Settings/SettingsManager.h>
#include <Utils/List.h>

namespace bp {

class ParaSurfacePlotter final : public Plotter3D {
    const SettingsManager& smPara;

    public:
    ParaSurfacePlotter(const SettingsManager& sm, const SettingsManager& smPara) : Plotter3D(sm), smPara{smPara} {}

    bool Plot(const tiparser::AST* func, Plot3D& plot, BaseColor color) const override {
        auto* tripleFunc = dynamic_cast<const tiparser::AST_Triple_Expr*>(func);
        if (tripleFunc == nullptr) {
            dbg_printf("Error: valid comma separated parametric equations were not inputted\n");
            return false;  // still allow the other plots to graph
        }
        const char* loadingString = "Loading vertices...";
        gfx_SetColor(0x00);
        gfx_SetDrawScreen();
        gfx_PrintStringXY(
            loadingString,
            GFX_LCD_WIDTH - gfx_GetStringWidth(loadingString) - TEXT_PADDING,
            GFX_LCD_HEIGHT - TEXT_HEIGHT - TEXT_PADDING);
        gfx_SetDrawBuffer();
        List<Point3D> points;
        int numRows = 0;
        int numCols = 0;
        uint8_t vars[2] = {OS_TOK_S, OS_TOK_T};
        double vVals[2];
        for (double t = smPara.GetDouble(TMIN); t <= smPara.GetDouble(TMAX);
             t += smPara.GetDouble(TSTEP)) {
            if (os_GetCSC() == sk_Clear) {
                while (!points.IsEmpty()) {
                    delete points.RemoveLast();  // more efficient to remove from back of the list
                }
                return true;
            }
            numRows++;
            numCols = 0;
            for (double s = smPara.GetDouble(SMIN); s <= smPara.GetDouble(SMAX);
                 s += smPara.GetDouble(SSTEP)) {
                vVals[0] = s;
                vVals[1] = t;
                double result[3];
                tripleFunc->TripleEval(2, vars, vVals, result);
                auto* p = new Point3D(result[0], result[1], result[2]);
                points.Add(p);
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
                    delete points.RemoveLast();
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
            delete points.RemoveLast();
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
