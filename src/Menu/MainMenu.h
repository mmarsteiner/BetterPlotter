//
// Created by mmars on 8/8/2024.
//

#pragma once

#include "MainMenuElements/MenuItem2DImplicit.h"
#include "MainMenuElements/MenuItemGeneric.h"
#include "MainMenuElements/MenuItemPlot3D.h"
#include "Menu.h"

namespace bp {
class MainMenu final : public Menu {
    MenuItem2DImplicit menuItem2DImplicit;
    MenuItemPlot3D menuItemPlot3D;

    MenuItemGeneric menuItem2DSettings;
    MenuItemGeneric menuItem3DSettings;
    MenuItemGeneric menuItemParaSettings;

    MenuItemGeneric menuItemClose;

    public:
    explicit MainMenu() : Menu("Better Plotter"), 
        menuItem2DSettings("2D Plot Settings", []() {
            Settings::GetRect2Settings().Open();
            return false;
        }),
        menuItem3DSettings("3D Plot Settings", []() {
            Settings::GetRect3Settings().Open();
            return false;
        }),
        menuItemParaSettings("Parametric Settings", []() {
            Settings::GetParaSettings().Open();
            return false;
        }),
        menuItemClose("Close", []() { return true; }) {

        AddMenuItem(menuItem2DImplicit);
        AddMenuItem(menuItemPlot3D);
        AddMenuItem(menuItem2DSettings);
        AddMenuItem(menuItem3DSettings);
        AddMenuItem(menuItemParaSettings);
        AddMenuItem(menuItemClose);
    }
};
}  // namespace bp
