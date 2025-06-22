#pragma once

#include <cstring>

#include <Menu/MenuItem.h>
#include <Parsing/AST.h>
#include <Parsing/Parser.h>
#include <Settings/Settings.h>
#include <Graphics/Renderer.h>
#include <Graphics/Axes.h>
#include <Graphics/BoundingBox.h>
#include <Graphics/Mesh.h>

namespace bp {

class MenuItemPlot3D final : public MenuItem {

    public:
    bool Run() override {
        tiparser::Parser parser;
        tiparser::AST& func = *parser.Parse(OS_VAR_Y1);
        tiparser::ExpressionType type = func.GetExpressionType();
        if (type == tiparser::EXP_TYPE_INCONSISTENT) {
            return false;
        }

        os_ClrHomeFull();
        gfx_Begin();
        gfx_SetColor(CUSTOM_BLACK);
        const char* loadingStr = "Loading...";
        gfx_PrintStringXY(loadingStr, GFX_LCD_WIDTH / 2 - gfx_GetStringWidth(loadingStr) / 2, GFX_LCD_HEIGHT / 2 - 5);

        g3d::Renderer renderer;
        renderer.Init3DColors();
        g3d::Axes axes;
        g3d::BoundingBox box;
        g3d::Mesh graphMesh;
        if (type == tiparser::EXP_TYPE_RECT || type == tiparser::EXP_TYPE_ANY) {
            const auto& settings = bp::Settings::GetRect3Settings();
            double xMin = settings.GetDouble(bp::XMIN_3);
            double xMax = settings.GetDouble(bp::XMAX_3);
            double xStep = settings.GetDouble(bp::XSTEP_3);
            double yMin = settings.GetDouble(bp::YMIN_3);
            double yMax = settings.GetDouble(bp::YMAX_3);
            double yStep = settings.GetDouble(bp::YSTEP_3);
            uint8_t vars[2] = {OS_TOK_X, OS_TOK_Y};
            double vals[2];
            for (double x = xMin; x < xMax; x += xStep) {
                graphMesh.AddRow();
                for (double y = yMin; y < yMax; y += yStep) {
                    vals[0] = x; vals[1] = y;
                    graphMesh.AddPoint(x, y, func.Eval(2, vars, vals));
                }
            }
        } else if (type == tiparser::EXP_TYPE_PARA) {
            auto& paraFunc = dynamic_cast<tiparser::AST_Triple_Expr&>(func);
            const auto& settings = bp::Settings::GetParaSettings();
            double uMin = settings.GetDouble(bp::UMIN);
            double uMax = settings.GetDouble(bp::UMAX);
            double uStep = settings.GetDouble(bp::USTEP);
            double vMin = settings.GetDouble(bp::VMIN);
            double vMax = settings.GetDouble(bp::VMAX);
            double vStep = settings.GetDouble(bp::VSTEP);
            uint8_t vars[2] = {OS_TOK_U, OS_TOK_V};
            double vals[2];
            double xyz[3];
            for (double u = uMin; u < uMax; u += uStep) {
                graphMesh.AddRow();
                for (double v = vMin; v < vMax; v += vStep) {
                    vals[0] = u; vals[1] = v;
                    paraFunc.TripleEval(2, vars, vals, xyz);
                    graphMesh.AddPoint(xyz[0], xyz[1], xyz[2]);
                }
            }
        }
        graphMesh.UpdateVisibility(renderer);
        graphMesh.UpdateColor(renderer);

        // actually draw the plot
        gfx_FillScreen(CUSTOM_WHITE);
        gfx_SetDrawBuffer();
        gfx_FillScreen(CUSTOM_WHITE);
        graphMesh.Render(renderer);
        axes.Render(renderer);
        box.Render(renderer);
        gfx_SwapDraw();

        // process rotations
        uint8_t key;
        int rotation = Settings::GetRect3Settings().GetDouble(bp::ROTATION_3);
        int elevation = Settings::GetRect3Settings().GetDouble(bp::ELEVATION_3);
        while((key = os_GetCSC()) != sk_Clear) {
            if (key == sk_Left || key == sk_Right || key == sk_Up || key == sk_Down) {
                if (key == sk_Left) {
                    rotation += 7;
                } else if (key == sk_Right) {
                    rotation -= 7;
                } else if (key == sk_Up) {
                    elevation += 7;
                } else /* key == sk_Down */ {
                    elevation -= 7;
                }
                renderer.UpdateVPSMatrix(rotation, elevation);
                gfx_FillScreen(CUSTOM_WHITE);
                graphMesh.Render(renderer);
                axes.Render(renderer);
                box.Render(renderer);
                gfx_SwapDraw();
            }
        }
        gfx_End();
        
        return false;
    }

    void GetLabel(char* out) override { strcpy(out, "Plot 3D"); }
    void AppendLabel(char* out) override { strcat(out, "Plot 3D"); }
};

}  // namespace bp
