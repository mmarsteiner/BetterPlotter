//
// Created by Matthew on 6/06/2024.
//

#include <Menu/MainMenu.h>
#include <Plot/Colors.h>
#include <Plot/Geometry.h>

int main() {
    tiparser::AST::InitOpPtrs();
    bp::InitCustomPalette();
    bp::MainMenu menu;
    menu.Run();
    while (os_GetCSC() != sk_Clear) {
    }
    return 0;
}
