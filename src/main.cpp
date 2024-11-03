//
// Created by Matthew on 6/06/2024.
//

#include <Menu/MainMenu.h>
#include <Plot/Colors.h>
#include <Plot/Geometry.h>
#include <Plot/TriangleRasterizer.h>

int main() {
    tiparser::AST::InitOpPtrs();
    /*gfx_Begin();
    bp::InitCustomPalette();
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 32; col++) {
            gfx_SetColor(row * 32 + col);
            gfx_FillRectangle(col * 10, row * 30, 10, 30);
        }
    }
    while(os_GetCSC() != sk_Clear) {}
    gfx_End();*/

    bp::MainMenu menu;
    menu.Run();
    return 0;
}
