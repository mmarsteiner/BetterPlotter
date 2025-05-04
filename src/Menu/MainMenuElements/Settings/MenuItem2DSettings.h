//
// Created by Matthew on 10/06/2024.
//

#pragma once

#include <Menu/MenuItem.h>
#include <Settings/Settings.h>

namespace bp {
class MenuItem2DSettings final : public MenuItem {
    public:

    bool Run() override {
        Settings::GetRect2Settings().Open();
        return false;
    }

    void GetLabel(char *out) override { strcpy(out, "2D Plot Settings"); }
    void AppendLabel(char *out) override { strcat(out, "2D Plot Settings"); }
};
}  // namespace bp