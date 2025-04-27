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
    while (os_GetCSC() != sk_Clear) {
    }
    gfx_End();
    bp::MainMenu menu;
    menu.Run();
    return 0;
}
