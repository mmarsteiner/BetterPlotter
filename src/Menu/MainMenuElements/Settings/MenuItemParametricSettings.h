//
// Created by mmars on 8/5/2024.
//

#pragma once

#include <cstring>

#define PARAMETRIC_SETTINGS_APPVAR_NAME "BPPAR"

namespace bp {

class MenuItemParametricSettings final : public MenuItem {
    SettingsManager* manager{};
    Stack<Setting> defaultSettings{};

   public:
    MenuItemParametricSettings() {
        Setting* defaults[] = {SettingDouble::Create("T Min", 0.0),
                               SettingDouble::Create("T Max", 2.0 * M_PI),
                               SettingDouble::Create("T Step", 0.1 * M_PI),
                               SettingDouble::Create("S Min", 0.0),
                               SettingDouble::Create("S Max", 2.0 * M_PI),
                               SettingDouble::Create("S Step", 0.1 * M_PI)};
        for (Setting* s : defaults) {
            defaultSettings.Push(s);
        }
        manager = new SettingsManager("Parametric Settings",
                                      PARAMETRIC_SETTINGS_APPVAR_NAME,
                                      &defaultSettings);
    }

    ~MenuItemParametricSettings() override {
        while (!defaultSettings.IsEmpty()) {
            delete defaultSettings.Pop();
        }
        delete manager;
    }

    const SettingsManager& GetSettingsManager() const { return *manager; }

    bool Run() override {
        manager->Open();
        return false;
    }

    void GetLabel(char* out) override { strcpy(out, "Parametric Settings"); }

    void AppendLabel(char* out) override { strcat(out, "Parametric Settings"); }
};

}  // namespace bp
