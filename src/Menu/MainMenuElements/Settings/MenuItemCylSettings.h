//
// Created by mmars on 8/5/2024.
//

#pragma once

#define SETTINGS_CYLINDRICAL_APPVAR_NAME "BPCYL"
#include <cmath>

namespace bp {

class MenuItemCylSettings final : public MenuItem {
    SettingsManager* manager{};
    Stack<Setting> defaultSettings{};

    public:
    const SettingsManager& GetSettingsManager() const { return *manager; }

    MenuItemCylSettings() {
        Setting* defaults[] = {SettingDouble::Create("Theta Min", 0.0),
                               SettingDouble::Create("Theta Max", 2.0 * M_PI),
                               SettingDouble::Create("Theta Step", 0.1 * M_PI),
                               SettingDouble::Create("Z Step", 0.5)};
        for (Setting* setting : defaults) {
            defaultSettings.Push(setting);
        }
        manager = new SettingsManager("Cylindrical Settings",
                                      SETTINGS_CYLINDRICAL_APPVAR_NAME,
                                      &defaultSettings);
    }

    ~MenuItemCylSettings() override {
        while (!defaultSettings.IsEmpty()) {
            delete defaultSettings.Pop();
        }
        delete manager;
    }

    bool Run() override {
        manager->Open();
        return false;
    }

    void GetLabel(char* out) override { strcpy(out, "Cylindrical Settings"); }

    void AppendLabel(char* out) override {
        strcat(out, "Cylindrical Settings");
    }
};

}  // namespace bp
