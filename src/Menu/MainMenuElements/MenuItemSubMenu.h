//
// Created by mmars on 8/1/2024.
//

#pragma once

#include <Menu/Menu.h>

namespace bp {

class MenuItemSubMenu final : public MenuItem {
    Menu& subMenu;

    public:
    explicit MenuItemSubMenu(Menu& subMenu) : subMenu{subMenu} {}

    bool Run() override {
        subMenu.Run();
        return false;
    }

    void GetLabel(char* out) override { subMenu.GetTitle(out); }

    void AppendLabel(char* out) override { subMenu.AppendTitle(out); }
};

}  // namespace bp
