//
// Created by Matthew on 6/06/2024.
//

#include <Menu/MainMenu.h>
#include <Plot/Colors.h>
#include <Plot/Geometry.h>
#include <Plot/TriangleRasterizer.h>

int main() {
    tiparser::AST::InitOpPtrs();
    gfx_Begin();
    bp::InitCustomPalette();
    bp::TriGFX test1 = {{186, 164, 0}, {120, 178, 0}, {204, 196, 0}};
    bp::TriGFX test2 = {{248, 152, 0}, {186, 164, 0}, {270, 182, 0}};
    bp::TriGFX test3 = {{134, 210, 0}, {204, 196, 0}, {120, 178, 0}};
    bp::TriGFX test4 = {{204, 196, 0}, {270, 182, 0}, {186, 164, 0}};
    while (os_GetCSC() != sk_Clear) {
    }
    bp::RasterizeTriScanlines(test1, 1 * 32);
    bp::RasterizeTriScanlines(test2, 2 * 32);
    bp::RasterizeTriScanlines(test3, 3 * 32);
    bp::RasterizeTriScanlines(test4, 4 * 32);
    while (os_GetCSC() != sk_Clear) {
    }
    gfx_End();

    bp::MainMenu menu;
    menu.Run();
    return 0;
}
