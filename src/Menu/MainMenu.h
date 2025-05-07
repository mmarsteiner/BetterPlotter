//
// Created by mmars on 8/8/2024.
//

#pragma once

#include "MainMenuElements/MenuItem2DImplicit.h"
#include "MainMenuElements/MenuItemClose.h"
#include "MainMenuElements/MenuItemSubMenu.h"
#include "MainMenuElements/Settings/MenuItem2DSettings.h"
#include "MainMenuElements/Settings/MenuItem3DCartesianSettings.h"
#include "MainMenuElements/Settings/MenuItem3DPlotColorSettings.h"
#include "MainMenuElements/Settings/MenuItemCylSettings.h"
#include "MainMenuElements/Settings/MenuItemParametricSettings.h"
#include "Menu.h"

namespace bp {
class MainMenu final : public Menu {
    MenuItem2DSettings menuItem2DSettings;
    MenuItem2DImplicit menuItem2DImplicit;
    MenuItem3DCartesianSettings menuItem3DCartesianSettings;
    MenuItemCylSettings menuItemCylSettings;
    MenuItemParametricSettings menuItemParametricSettings;
    MenuItem3DPlotColorSettings menuItem3DPlotColorSettings;
    
    Menu settingsSelectionMenu{"Settings"};
    MenuItemSubMenu settingsSubMenu{settingsSelectionMenu};
    MenuItemClose menuItemClose;

    public:
    explicit MainMenu() : Menu("Better Plotter") {
        settingsSelectionMenu.AddMenuItem(menuItem2DSettings);
        settingsSelectionMenu.AddMenuItem(menuItem3DPlotColorSettings);
        settingsSelectionMenu.AddMenuItem(menuItem3DCartesianSettings);
        settingsSelectionMenu.AddMenuItem(menuItemCylSettings);
        settingsSelectionMenu.AddMenuItem(menuItemParametricSettings);
        AddMenuItem(menuItem2DImplicit);
        AddMenuItem(settingsSubMenu);
        AddMenuItem(menuItemClose);
    }
};
}  // namespace bp
