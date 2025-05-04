//
// Created by mmars on 8/5/2024.
//

#pragma once

#include <Menu/MenuItem.h>
#include <Settings/Settings.h>

namespace bp {

class MenuItemParametricSettings final : public MenuItem {
    public:

    bool Run() override {
        Settings::GetParaSettings().Open();
        return false;
    }

    void GetLabel(char* out) override { strcpy(out, "Parametric Settings"); }
    void AppendLabel(char* out) override { strcat(out, "Parametric Settings"); }
};

}  // namespace bp
