//
// Created by Matthew on 6/06/2024.
//

#include <Menu/MainMenu.h>
#include <Plot/Colors.h>
#include <Settings/Settings.h>
#include <Graphics/Renderer.h>
#include <graphx.h>

int main() {
    bp::Settings::InitSettings();
    tiparser::AST::InitOpPtrs();
    bp::InitCustomPalette();
    //bp::MainMenu menu;
    //menu.Run();

    // Graphics test code that displays the coordinate axes rotating in the xy plane
    bp::SettingsManager& s3 = bp::Settings::GetRect3Settings();
    g3d::Vec4 xMax(s3.GetDouble(bp::XMAX_3), 0, 0, 1);
    g3d::Vec4 xMin(s3.GetDouble(bp::XMIN_3), 0, 0, 1);
    g3d::Vec4 yMax(0, s3.GetDouble(bp::YMAX_3), 0, 1);
    g3d::Vec4 yMin(0, s3.GetDouble(bp::YMIN_3), 0, 1);
    g3d::Vec4 zMax(0, 0, s3.GetDouble(bp::ZMAX_3), 1);
    g3d::Vec4 zMin(0, 0, s3.GetDouble(bp::ZMIN_3), 1);

    double elevation = s3.GetDouble(bp::ELEVATION_3);

    os_ClrHomeFull();
    gfx_Begin();
    gfx_SetDrawBuffer();
    g3d::Renderer renderer;
    for (int rotation = 0; rotation < 360; ++rotation) {
        if (os_GetCSC() == sk_Clear) {
            break;
        }

        renderer.UpdateVPSMatrix(rotation, elevation);

        gfx_FillScreen(gfx_white);
        gfx_SetColor(gfx_red);
        renderer.RenderLine(xMin, xMax);
        gfx_SetColor(gfx_green);
        renderer.RenderLine(yMin, yMax);
        gfx_SetColor(gfx_blue);
        renderer.RenderLine(zMin, zMax);

        gfx_SwapDraw();
    }
    // end test code

    while (os_GetCSC() != sk_Clear) {
    }

    gfx_End();
    bp::Settings::CleanupSettings();
    return 0;
}
