//
// Created by Matthew on 9/06/2024.
//

#pragma once

#include <Utils/stack.h>
#include <Utils/utils.h>
#include <debug.h>
#include <fileioc.h>
#include <ti/flags.h>
#include <ti/screen.h>

#include <cstddef>

#include "Setting.h"

namespace bp {
class SettingsManager {
    char title[20]{};
    char appVarName[20]{};
    uint8_t selectedIdx = 0;
    uint8_t scrollOffset = 0;
    Stack<Setting> settings{};
    Stack<Setting> defaultSettings{};

    Setting *GetSelectedSetting() const;

    uint8_t GetSelectedRow() const { return selectedIdx - scrollOffset + 1; }

    void DrawMenu() const;

    void StoreSettings() const;

    void RetrieveSettings();

    void OverwriteWithDefaults();

    public:
    SettingsManager(const char *title, const char *appVarName,
                    const Stack<Setting> *defaultSettings);

    ~SettingsManager();

    void Open();

    double GetDouble(int idx) const {
        return dynamic_cast<SettingDouble *>(settings.Get(idx))->val;
    }

    uint32_t GetUInt(int idx) const {
        return dynamic_cast<SettingUInt *>(settings.Get(idx))->val;
    }

    bool GetBool(int idx) const {
        return dynamic_cast<SettingBool *>(settings.Get(idx))->val;
    }

    Stack<Setting> *GetSettings() { return &settings; }

    void TestSettings() const;
};
}  // namespace bp
