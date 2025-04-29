//
// Created by mmars on 10/6/2024.
//

#pragma once

#include <Utils/List.h>

#include "../../../Plot/Colors.h"
#include "../../../Settings/Setting.h"
#include "../../../Settings/SettingsManager.h"
#include "../../MenuItem.h"

#define SETTINGS_3D_PLOTS "BP3DPS"

namespace bp {

enum PlotType3D {
    OFF,
    CARTESIAN,
    CYLINDRICAL,
    PARA_SURF
};

class MenuItem3DPlotColorSettings final : public MenuItem {
    SettingsManager* manager;
    List<Setting> defaultSettings;

    public:
    const SettingsManager& GetSettingsManager() const { return *manager; }
    MenuItem3DPlotColorSettings() {
        Setting* defaults[] = {
            SettingUInt::Create("Y1 Type", CARTESIAN),  // Cartesian plot
            SettingUInt::Create("Y1 Color", BLUE),
            SettingUInt::Create("Y2 Type", OFF),
            SettingUInt::Create("Y2 Color", RED),
            SettingUInt::Create("Y3 Type", OFF),
            SettingUInt::Create("Y3 Color", 3),
            SettingUInt::Create("Y4 Type", OFF),
            SettingUInt::Create("Y4 Color", 4),
            SettingUInt::Create("Y5 Type", OFF),
            SettingUInt::Create("Y5 Color", 5),
            SettingUInt::Create("Y6 Type", OFF),
            SettingUInt::Create("Y6 Color", 6),
            SettingUInt::Create("Y7 Type", OFF),
            SettingUInt::Create("Y7 Color", 0)};
        defaultSettings.AddAll(defaults, 14);
        manager = new SettingsManager("3D Plot & Color Selection", SETTINGS_3D_PLOTS, &defaultSettings);
    }

    ~MenuItem3DPlotColorSettings() override {
        delete manager;
        while (!defaultSettings.IsEmpty()) {
            delete defaultSettings.RemoveLast();
        }
    }

    bool Run() override {
        manager->Open();
        return false;
    }

    void GetLabel(char* out) override { strcpy(out, "3D Plot & Color Settings\n"); }

    void AppendLabel(char* out) override { strcat(out, "3D Plot & Color Settings\n"); }
};

}  // namespace bp
