//
// Created by Matthew on 9/06/2024.
//

#include "SettingsManager.h"

#include <debug.h>

#include <typeinfo>

namespace bp {
SettingsManager::SettingsManager(const char *title, const char *appVarName,
                                 const Stack<Setting> *defaultSettings) {
    strncpy(this->title, title, SETTING_LABEL_MAX);
    strncpy(this->appVarName, appVarName, SETTING_LABEL_MAX);
    for (size_t i = 0; i < defaultSettings->Size(); i++) {
        this->defaultSettings.Push(defaultSettings->Get(i)->Copy());
    }
    RetrieveSettings();
}

SettingsManager::~SettingsManager() {
    for (size_t i = 0; i < settings.Size(); i++) {
        delete settings.Pop();
    }
    for (size_t i = 0; i < defaultSettings.Size(); i++) {
        delete defaultSettings.Pop();
    }
}

Setting *SettingsManager::GetSelectedSetting() const {
    return settings.Get(selectedIdx);
}

void SettingsManager::DrawMenu() const {
    os_DisableCursor();
    os_ClrHomeFull();
    os_ClrTxtShd();
    uint8_t currRow = 0;
    os_SetCursorPos(currRow, 0);
    os_PutStrFull(title);
    for (size_t i = scrollOffset;
         i < settings.Size() && i - scrollOffset + 1 <= 9; i++) {
        os_SetCursorPos(++currRow, 1);
        Setting *s = settings.Get(i);
        char line[SETTING_LABEL_MAX + 10];
        s->GetScreenLine(line);
        os_PutStrFull(line);
    }
    os_SetCursorPos(GetSelectedRow(), GetSelectedSetting()->GetCursorCol());
    os_EnableCursor();
}

void SettingsManager::StoreSettings() const {
    uint16_t bytesToStore = 0;
    for (int i = 0; static_cast<size_t>(i) < settings.Size(); i++) {
        bytesToStore += settings.Get(i)->GetValSize();
    }
    auto *bytes = new unsigned char[bytesToStore];
    unsigned char *writePosition = bytes;
    for (size_t i = 0; i < settings.Size(); i++) {
        settings.Get(i)->GetVal(writePosition);
        writePosition += settings.Get(i)->GetValSize();
    }
    uint8_t settingsHandle = ti_Open(appVarName, "w");
    ti_Write(bytes, 1, bytesToStore, settingsHandle);
    ti_Close(settingsHandle);

    delete[] bytes;
}

void SettingsManager::RetrieveSettings() {
    uint8_t settingsHandle = ti_Open(appVarName, "r");
    OverwriteWithDefaults();
    if (settingsHandle == 0) {
        // settings appvar does not already exist
        StoreSettings();
    } else {
        uint16_t totalBytes = ti_GetSize(settingsHandle);
        auto *bytes = new unsigned char[totalBytes];
        ti_Read(bytes, 1, totalBytes, settingsHandle);
        uint16_t offset = 0;

        uint16_t expectedTotalBytes = 0;
        for (size_t i = 0; i < settings.Size(); i++) {
            expectedTotalBytes += settings.Get(i)->GetValSize();
        }
        if (expectedTotalBytes != totalBytes) {
            dbg_printf("Expected %d bytes in %s but found %d bytes\n",
                       expectedTotalBytes, title, totalBytes);
            ti_Close(settingsHandle);
            StoreSettings();  // replace invalid data with valid data from
                              // default settings
            return;
        }
        for (size_t i = 0; i < settings.Size(); i++) {
            Setting *toUpdate = settings.Get(i);
            toUpdate->SetVal(bytes + offset);
            offset += toUpdate->GetValSize();
        }
        ti_Close(settingsHandle);
        delete[] bytes;
    }
}

void SettingsManager::OverwriteWithDefaults() {
    while (!settings.IsEmpty()) {
        delete settings.Pop();
    }
    for (size_t i = 0; i < defaultSettings.Size(); i++) {
        settings.Push(defaultSettings.Get(i)->Copy());
    }
}

void SettingsManager::TestSettings() const {
    dbg_printf("--------------\n");
    dbg_printf("Settings:");
    for (size_t i = 0; i < settings.Size(); i++) {
        char dbg[50];
        settings.Get(i)->GetScreenLine(dbg);
        dbg_printf("%s\n", dbg);
    }
    dbg_printf("--------------\n");
}

void SettingsManager::Open() {
    scrollOffset = 0;
    selectedIdx = 0;
    DrawMenu();
    sk_key_t key;
    GetSelectedSetting()->StartEditing();
    while ((key = os_GetCSC()) != sk_Clear) {
        GetSelectedSetting()->ProcessInput(key);
        if (key == sk_Down || key == sk_Enter) {
            if (selectedIdx < settings.Size() - 1) {
                selectedIdx++;
                os_DisableCursor();
                if (GetSelectedRow() <= 9) {
                    os_SetCursorPos(GetSelectedRow(),
                                    GetSelectedSetting()->GetCursorCol());
                } else {
                    os_ClrHomeFull();
                    scrollOffset++;
                    DrawMenu();
                }
                GetSelectedSetting()->StartEditing();
                os_EnableCursor();
            } else {
                selectedIdx = 0;
                scrollOffset = 0;
                os_DisableCursor();
                os_ClrHomeFull();
                GetSelectedSetting()->StartEditing();
                DrawMenu();
            }
        } else if (key == sk_Up) {
            if (selectedIdx > 0) {
                selectedIdx--;
                os_DisableCursor();
                if (GetSelectedRow() != 0) {
                    os_SetCursorPos(GetSelectedRow(),
                                    GetSelectedSetting()->GetCursorCol());
                } else {
                    os_ClrHomeFull();
                    scrollOffset--;
                    DrawMenu();
                }
                GetSelectedSetting()->StartEditing();
                os_EnableCursor();
            } else {
                selectedIdx = settings.Size() - 1;
                scrollOffset = settings.Size() - 9;
                os_DisableCursor();
                os_ClrHomeFull();
                DrawMenu();
                GetSelectedSetting()->StartEditing();
            }
        }
    }
    GetSelectedSetting()->editingOffset = 0;
    StoreSettings();
}
}  // namespace bp
