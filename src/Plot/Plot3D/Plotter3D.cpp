#include "Plotter3D.h"

namespace bp {

void Plotter3D::DrawLoadingString() const {
    gfx_SetDrawScreen();
    gfx_SetColor(0x00);
    gfx_PrintStringXY(
        LOADING_STRING,
        GFX_LCD_WIDTH - gfx_GetStringWidth(LOADING_STRING) - TEXT_PADDING,
        GFX_LCD_HEIGHT - TEXT_HEIGHT - TEXT_PADDING);
    gfx_SetDrawBuffer();
}

void Plotter3D::DrawRenderingString() const {
    gfx_SetColor(0xFF);
    gfx_SetDrawScreen();
    gfx_FillRectangle(
        GFX_LCD_WIDTH - gfx_GetStringWidth(LOADING_STRING) - TEXT_PADDING,
        GFX_LCD_HEIGHT - TEXT_HEIGHT - TEXT_PADDING,
        gfx_GetStringWidth(LOADING_STRING), TEXT_HEIGHT);
    gfx_SetColor(0x00);
    gfx_PrintStringXY(
        RENDERING_STRING,
        GFX_LCD_WIDTH - gfx_GetStringWidth(RENDERING_STRING) - TEXT_PADDING,
        GFX_LCD_HEIGHT - TEXT_HEIGHT - TEXT_PADDING);
    gfx_SetDrawBuffer();
}

void Plotter3D::ClearRenderingString() const {
    gfx_SetColor(0xFF);
    gfx_SetDrawScreen();
    gfx_FillRectangle(
        GFX_LCD_WIDTH - gfx_GetStringWidth(RENDERING_STRING) - TEXT_PADDING,
        GFX_LCD_HEIGHT - TEXT_HEIGHT - TEXT_PADDING,
        gfx_GetStringWidth(RENDERING_STRING), TEXT_HEIGHT);
    gfx_SetDrawBuffer();
    gfx_SetColor(0x00);
}

bool Plotter3D::GeneratePointMesh(PointMesh& output, VariableRange var1, VariableRange var2, const tiparser::AST* func) const {
    for (double a = var1.min; a <= var1.max; a += var1.step) {
        ++output.width;
        output.height = 0;
        if (os_GetCSC() == sk_Clear) {
            return true;
        }
        for (double b = var2.min; b <= var2.max; b += var2.step) {
            ++output.height;
            Point3D* point = CreatePoint(func, a, b);
            output.points.Add(point);
        }
    }
    return false;
}

bool Plotter3D::PlotPoints(PointMesh mesh, Plot3D& plot, BaseColor color) const {
    int meshSize = mesh.width * mesh.height;
    uint8_t contourColor = color;
    if (sm.GetBool(SHADE_SURFACE)) {
        contourColor = 0x00;
    }
    for (int i = 0; i < static_cast<int>(mesh.points.Size()); i++) {
        if (os_GetCSC() == sk_Clear) {
            return true;
        }
        Point3D* corner = mesh.points.Get(i);
        if (sm.GetBool(DRAW_CONTOUR_LINES)) {
            if (i % mesh.height != 0) {
                plot.DrawZClippedLine({*corner, *mesh.points.Get(i - 1)}, contourColor);
            }
            if (i - mesh.height >= 0) {
                plot.DrawZClippedLine({*corner, *mesh.points.Get(i - mesh.height)}, contourColor);
            }
        }
        if (sm.GetBool(SHADE_SURFACE)) {
            if (i % mesh.height != 0 && i + mesh.height < meshSize) {
                plot.ShadeTri({*corner, *mesh.points.Get(i - 1), *mesh.points.Get(i + mesh.height)}, color);
            }
            if (i % mesh.height != mesh.height - 1 && i - mesh.height >= 0) {
                plot.ShadeTri({*corner, *mesh.points.Get(i + 1), *mesh.points.Get(i - mesh.height)}, color);
            }
        }
    }
    return false;
}

bool Plotter3D::Plot(const tiparser::AST* func, Plot3D& plot, BaseColor color) const {
    DrawLoadingString();
    List<Point3D> points;
    PointMesh mesh{points, 0, 0};
    VariableRange var1{0, 0, 0, 0}, var2{0, 0, 0, 0};  // temporary values; if these are used there is a problem
    ExtractVariableSettings(var1, var2);
    bool interrupted = GeneratePointMesh(mesh, var1, var2, func);
    if (interrupted) {
        mesh.ClearMesh();
        return true;
    }
    DrawRenderingString();
    interrupted = PlotPoints(mesh, plot, color);
    ClearRenderingString();
    mesh.ClearMesh();
    return interrupted;
}

void Plotter3D::PointMesh::ClearMesh() {
    while (!points.IsEmpty()) {
        delete points.RemoveLast();
    }
}

}  // namespace bp