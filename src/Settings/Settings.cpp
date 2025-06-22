#include "Settings.h"

namespace bp {
    SettingsManager* Settings::rect2Sm = nullptr;
    SettingsManager* Settings::rect3Sm = nullptr;
    SettingsManager* Settings::paraSm = nullptr;

    void Settings::InitSettings()  {
        List<Setting> defaultRect2;
        defaultRect2.Add(SettingDouble::Create("xMin", -10.0));
        defaultRect2.Add(SettingDouble::Create("xMax", 10.0));
        defaultRect2.Add(SettingDouble::Create("yMin", -10.0));
        defaultRect2.Add(SettingDouble::Create("yMax", 10.0));
        defaultRect2.Add(SettingBool::Create("Draw Ticks", true));
        defaultRect2.Add(SettingDouble::Create("X Tick Intervals", 1.0));
        defaultRect2.Add(SettingDouble::Create("Y Tick Intervals", 1.0));
        defaultRect2.Add(SettingBool::Create("Draw Grey Lines", true));
        defaultRect2.Add(SettingUInt::Create("Min Recursion", 3));
        defaultRect2.Add(SettingUInt::Create("Max Recursion", 6));
        defaultRect2.Add(SettingBool::Create("Debug Regions", false));
        rect2Sm = new SettingsManager("2D Plot Settings", SETTINGS_2D_APPVAR_NAME, &defaultRect2);
        while (!defaultRect2.IsEmpty()) {
            delete defaultRect2.RemoveLast();
        }
        List<Setting> defaultRect3;
        defaultRect3.Add(SettingDouble::Create("xMin", -5.0));
        defaultRect3.Add(SettingDouble::Create("xMax", 5.0));
        defaultRect3.Add(SettingDouble::Create("yMin", -5.0));
        defaultRect3.Add(SettingDouble::Create("yMax", 5.0));
        defaultRect3.Add(SettingDouble::Create("zMin", -5.0));
        defaultRect3.Add(SettingDouble::Create("zMax", 5.0));
        defaultRect3.Add(SettingDouble::Create("xStep", 0.5));
        defaultRect3.Add(SettingDouble::Create("yStep", 0.5));
        defaultRect3.Add(SettingDouble::Create("Elevation", 35.26));
        defaultRect3.Add(SettingDouble::Create("Rotation", -67.5));
        rect3Sm = new SettingsManager("3D Plot Settings", SETTINGS_3D_RECT_APPVAR_NAME, &defaultRect3);
        while (!defaultRect3.IsEmpty()) {
            delete defaultRect3.RemoveLast();
        }
        List<Setting> defaultPara;
        defaultPara.Add(SettingDouble::Create("U Min", 0.0));
        defaultPara.Add(SettingDouble::Create("U Max", 2.0 * M_PI));
        defaultPara.Add(SettingDouble::Create("U Step", 0.1 * M_PI));
        defaultPara.Add(SettingDouble::Create("V Min", 0.0));
        defaultPara.Add(SettingDouble::Create("V Max", 2.0 * M_PI));
        defaultPara.Add(SettingDouble::Create("V Step", 0.1 * M_PI));
        paraSm = new SettingsManager("Parametric Settings", SETTINGS_PARAMETRIC_APPVAR_NAME, &defaultPara);
        while (!defaultPara.IsEmpty()) {
            delete defaultPara.RemoveLast();
        }
    }
}