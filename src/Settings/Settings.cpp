#include "Settings.h"

namespace bp {
    SettingsManager* Settings::rect2Sm = nullptr;
    SettingsManager* Settings::rect3Sm = nullptr;
    SettingsManager* Settings::cpSm = nullptr;
    SettingsManager* Settings::cylSm = nullptr;
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
        defaultRect3.Add(SettingBool::Create("Draw Contour Lines", true));
        defaultRect3.Add(SettingBool::Create("Shade Surface", true));
        defaultRect3.Add(SettingBool::Create("Draw Outer Box", true));
        rect3Sm = new SettingsManager("3D Plot Settings", SETTINGS_3D_RECT_APPVAR_NAME, &defaultRect3);
        while (!defaultRect3.IsEmpty()) {
            delete defaultRect3.RemoveLast();
        }
        List<Setting> defaultCP;
        defaultCP.Add(SettingUInt::Create("Y1 Type", CARTESIAN));
        defaultCP.Add(SettingUInt::Create("Y1 Color", BLUE));
        defaultCP.Add(SettingUInt::Create("Y2 Type", OFF));
        defaultCP.Add(SettingUInt::Create("Y2 Color", RED));
        defaultCP.Add(SettingUInt::Create("Y3 Type", OFF));
        defaultCP.Add(SettingUInt::Create("Y3 Color", 3));
        defaultCP.Add(SettingUInt::Create("Y4 Type", OFF));
        defaultCP.Add(SettingUInt::Create("Y4 Color", 4));
        defaultCP.Add(SettingUInt::Create("Y5 Type", OFF));
        defaultCP.Add(SettingUInt::Create("Y5 Color", 5));
        defaultCP.Add(SettingUInt::Create("Y6 Type", OFF));
        defaultCP.Add(SettingUInt::Create("Y6 Color", 6));
        defaultCP.Add(SettingUInt::Create("Y7 Type", OFF));
        defaultCP.Add(SettingUInt::Create("Y7 Color", 0));
        cpSm = new SettingsManager("Plot/Color Settings", SETTINGS_3D_PLOTS_APPVAR_NAME, &defaultCP);
        while (!defaultCP.IsEmpty()) {
            delete defaultCP.RemoveLast();
        }
        List<Setting> defaultCyl;
        defaultCyl.Add(SettingDouble::Create("Theta Min", 0.0));
        defaultCyl.Add(SettingDouble::Create("Theta Max", 2.0 * M_PI));
        defaultCyl.Add(SettingDouble::Create("Theta Step", 0.1 * M_PI));
        defaultCyl.Add(SettingDouble::Create("Z Step", 0.5));
        cylSm = new SettingsManager("Cylindrical Settings", SETTINGS_CYLINDRICAL_APPVAR_NAME, &defaultCyl);
        while (!defaultCyl.IsEmpty()) {
            delete defaultCyl.RemoveLast();
        }
        List<Setting> defaultPara;
        defaultPara.Add(SettingDouble::Create("T Min", 0.0));
        defaultPara.Add(SettingDouble::Create("T Max", 2.0 * M_PI));
        defaultPara.Add(SettingDouble::Create("T Step", 0.1 * M_PI));
        defaultPara.Add(SettingDouble::Create("S Min", 0.0));
        defaultPara.Add(SettingDouble::Create("S Max", 2.0 * M_PI));
        defaultPara.Add(SettingDouble::Create("S Step", 0.1 * M_PI));
        paraSm = new SettingsManager("Parametric Settings", SETTINGS_PARAMETRIC_APPVAR_NAME, &defaultPara);
        while (!defaultPara.IsEmpty()) {
            delete defaultPara.RemoveLast();
        }
    }
}