//
// Created by Matthew on 10/06/2024.
//

#pragma once

#include <Menu/MenuItem.h>
#include <Settings/SettingsManager.h>
#include <Utils/List.h>

#define SETTINGS_2D_APPVAR_NAME "BP2D"

namespace bp {
class MenuItem2DSettings final : public MenuItem {
    SettingsManager *manager;
    List<Setting> defaultSettings{};

    public:
    const SettingsManager &GetSettingsManager() const { return *manager; }

    MenuItem2DSettings() {
        Setting *defaults[] = {SettingDouble::Create("xMin", -10.0),
                               SettingDouble::Create("xMax", 10.0),
                               SettingDouble::Create("yMin", -10.0),
                               SettingDouble::Create("yMax", 10.0),
                               SettingBool::Create("Draw Ticks", true),
                               SettingDouble::Create("X Tick Intervals", 1.0),
                               SettingDouble::Create("Y Tick Intervals", 1.0),
                               SettingBool::Create("Draw Grey Lines", true),
                               SettingUInt::Create("Min Recursion", 3),
                               SettingUInt::Create("Max Recursion", 6),
                               SettingBool::Create("Debug Regions", false)};
        defaultSettings.AddAll(defaults, 11);

        manager = new SettingsManager(
            "2D Plot Settings", SETTINGS_2D_APPVAR_NAME, &defaultSettings);
    }

    ~MenuItem2DSettings() override {
        delete manager;
        while (!defaultSettings.IsEmpty()) {
            delete defaultSettings.RemoveLast();
        }
    };

    bool Run() override {
        manager->Open();
        return false;
    }

    void GetLabel(char *out) override { strcpy(out, "2D Plot Settings"); }

    void AppendLabel(char *out) override { strcat(out, "2D Plot Settings"); }
};
}  // namespace bp