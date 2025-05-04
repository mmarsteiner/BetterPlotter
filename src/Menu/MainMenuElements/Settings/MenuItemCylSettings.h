//
// Created by mmars on 8/5/2024.
//

#pragma once

#include <Menu/MenuItem.h>
#include <Settings/Settings.h>

namespace bp {

class MenuItemCylSettings final : public MenuItem {
    public:

    bool Run() override {
        Settings::GetCylSettings().Open();
        return false;
    }

    void GetLabel(char* out) override { strcpy(out, "Cylindrical Settings"); }

    void AppendLabel(char* out) override {
        strcat(out, "Cylindrical Settings");
    }
};

}  // namespace bp
