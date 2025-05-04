//
// Created by mmars on 8/1/2024.
//

#pragma once

#include <Menu/MenuItem.h>
#include <Settings/Settings.h>

namespace bp {
class MenuItem3DCartesianSettings final : public MenuItem {
    public:

    bool Run() override {
        Settings::GetRect3Settings().Open();
        return false;
    }

    void GetLabel(char* out) override { strcpy(out, "Cartesian Plot Settings\n"); }
    void AppendLabel(char* out) override { strcat(out, "Cartesian Plot Settings\n"); }
};
}  // namespace bp
