//
// Created by Matthew on 6/06/2024.
//

#include <Menu/MainMenu.h>
#include <Settings/Settings.h>
#include <Graphics/Renderer.h>
#include <Graphics/Axes.h>
#include <Graphics/BoundingBox.h>
#include <graphx.h>
#include <Graphics/Mesh.h>
#include <cmath>

int main() {
    bp::Settings::InitSettings();
    tiparser::AST::InitOpPtrs();
    //bp::MainMenu menu;
    //menu.Run();

    // Graphics test code that displays a plot rotating in 3D space
    bp::SettingsManager& s3 = bp::Settings::GetRect3Settings();
    g3d::Vec4 xMax(s3.GetDouble(bp::XMAX_3), 0, 0, 1);
    g3d::Vec4 xMin(s3.GetDouble(bp::XMIN_3), 0, 0, 1);
    g3d::Vec4 yMax(0, s3.GetDouble(bp::YMAX_3), 0, 1);
    g3d::Vec4 yMin(0, s3.GetDouble(bp::YMIN_3), 0, 1);
    g3d::Vec4 zMax(0, 0, s3.GetDouble(bp::ZMAX_3), 1);
    g3d::Vec4 zMin(0, 0, s3.GetDouble(bp::ZMIN_3), 1);

    double elevation = s3.GetDouble(bp::ELEVATION_3);
    double xStep = s3.GetDouble(bp::XSTEP_3);
    double yStep = s3.GetDouble(bp::YSTEP_3);

    g3d::Renderer renderer;

    g3d::Mesh testMesh;
    for (double x = xMin.x; x <= xMax.x; x += xStep) {
        testMesh.AddRow();
        for (double y = yMin.y; y <= yMax.y; y += yStep) {
            testMesh.AddPoint(x, y, sin((M_PI * x) / 4.0) * sin((M_PI * y) / 4.0));
        }
    }
    testMesh.UpdateVisibility(renderer);
    testMesh.UpdateColor(renderer);
    g3d::Axes axes;
    g3d::BoundingBox boundingBox;

    os_ClrHomeFull();
    gfx_Begin();
    renderer.Init3DColors();

    gfx_SetDrawBuffer();
    gfx_FillScreen(CUSTOM_WHITE);
    testMesh.Render(renderer);
    axes.Render(renderer);
    boundingBox.Render(renderer);
    gfx_SwapDraw();

    // process rotations
    uint8_t key;
    int rotation = -67;
    while((key = os_GetCSC()) != sk_Clear) {
        if (key == sk_Left || key == sk_Right) {
            if (key == sk_Left) {
                rotation -= 7;
                renderer.UpdateVPSMatrix(rotation, elevation);
            } else if (key == sk_Right) {
                rotation += 7;
                renderer.UpdateVPSMatrix(rotation, elevation);
            }
            gfx_FillScreen(CUSTOM_WHITE);
            testMesh.Render(renderer);
            axes.Render(renderer);
            boundingBox.Render(renderer);
            gfx_SwapDraw();
        }
    }
    // end test code

    gfx_End();
    bp::Settings::CleanupSettings();
    return 0;
}
