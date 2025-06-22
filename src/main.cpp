//
// Created by Matthew on 6/06/2024.
//

#include <Menu/MainMenu.h>
#include <Settings/Settings.h>

int main() {
    bp::Settings::InitSettings();
    tiparser::AST::InitOpPtrs();
    bp::MainMenu menu;
    menu.Run();
    bp::Settings::CleanupSettings();
    return 0;
}
