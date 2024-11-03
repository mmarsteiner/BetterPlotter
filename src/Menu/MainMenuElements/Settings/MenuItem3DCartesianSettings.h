//
// Created by mmars on 8/1/2024.
//

#pragma once

#define SETTINGS_3D_CART_APPVAR_NAME "BP3DC"

namespace bp {
class MenuItem3DCartesianSettings final : public MenuItem {
    SettingsManager* manager;
    Stack<Setting> defaultSettings{};

  public:
    const SettingsManager& GetSettingsManager() const { return *manager; }

    MenuItem3DCartesianSettings() {
        Setting* defaults[] = {
            SettingDouble::Create("xMin", -5.0),
            SettingDouble::Create("xMax", 5.0),
            SettingDouble::Create("yMin", -5.0),
            SettingDouble::Create("yMax", 5.0),
            SettingDouble::Create("zMin", -5.0),
            SettingDouble::Create("zMax", 5.0),
            SettingDouble::Create("xStep", 0.5),
            SettingDouble::Create("yStep", 0.5),
            SettingDouble::Create("Elevation", 35.26),
            SettingDouble::Create("Rotation", -67.5),
            SettingBool::Create("Draw Contour Lines", true),
            SettingBool::Create("Shade Surface", true),
            SettingBool::Create("Draw Outer Box", true),
        };
        for (Setting* s : defaults) {
            defaultSettings.Push(s);
        }
        manager = new SettingsManager(
            "Cartesian Plot Settings", SETTINGS_3D_CART_APPVAR_NAME, &defaultSettings);
    }

    ~MenuItem3DCartesianSettings() override {
        delete manager;
        while (!defaultSettings.IsEmpty()) {
            delete defaultSettings.Pop();
        }
    }

    bool Run() override {
        manager->Open();
        return false;
    }

    void GetLabel(char* out) override { strcpy(out, "Cartesian Plot Settings\n"); }

    void AppendLabel(char* out) override { strcat(out, "Cartesian Plot Settings\n"); }
};
}  // namespace bp
