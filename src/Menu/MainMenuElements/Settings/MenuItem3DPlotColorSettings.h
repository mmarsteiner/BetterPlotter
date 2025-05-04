//
// Created by mmars on 10/6/2024.
//

#pragma once

#include <Settings/Settings.h>
#include <Menu/MenuItem.h>

namespace bp {

class MenuItem3DPlotColorSettings final : public MenuItem {
    public:

    bool Run() override {
        Settings::GetColorPlotSettings().Open();
        return false;
    }

    void GetLabel(char* out) override { strcpy(out, "3D Plot & Color Settings\n"); }
    void AppendLabel(char* out) override { strcat(out, "3D Plot & Color Settings\n"); }
};

}  // namespace bp
