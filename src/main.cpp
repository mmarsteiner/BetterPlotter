//
// Created by Matthew on 6/06/2024.
//

#include <Menu/MainMenu.h>
#include <Plot/Colors.h>
#include <Settings/Settings.h>
#include <debug.h>

int main() {
    dbg_printf("Calling InitSettings()\n");
    bp::Settings::InitSettings();
    dbg_printf("InitSettings() returned\n");
    tiparser::AST::InitOpPtrs();
    bp::InitCustomPalette();
    bp::MainMenu menu;
    menu.Run();
    while (os_GetCSC() != sk_Clear) {
    }
    bp::Settings::CleanupSettings();
    return 0;
}
