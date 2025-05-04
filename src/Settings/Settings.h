#pragma once

#include <Settings/SettingsManager.h>
#include <Plot/Colors.h>
#include <cmath>
#include <debug.h>

#define SETTINGS_2D_APPVAR_NAME "BP2D"
#define SETTINGS_3D_RECT_APPVAR_NAME "BP3DR"
#define SETTINGS_3D_PLOTS_APPVAR_NAME "BP3DPS"
#define SETTINGS_CYLINDRICAL_APPVAR_NAME "BPCYL"
#define SETTINGS_PARAMETRIC_APPVAR_NAME "BPPAR"

namespace bp {

enum PlotType3D {
    OFF,
    CARTESIAN,
    CYLINDRICAL,
    PARA_SURF
};

class Settings {
    static SettingsManager* rect2Sm;
    static SettingsManager* rect3Sm;
    static SettingsManager* cpSm;
    static SettingsManager* cylSm;
    static SettingsManager* paraSm;

    public:
    static SettingsManager& GetRect2Settings() { return *rect2Sm; }
    static SettingsManager& GetRect3Settings() { return *rect3Sm; }
    static SettingsManager& GetColorPlotSettings() { return *cpSm; }
    static SettingsManager& GetCylSettings() { return *cylSm; }
    static SettingsManager& GetParaSettings() { return *paraSm; }

    static void InitSettings();

    static void CleanupSettings() {
        delete rect2Sm;
        delete rect3Sm;
        delete cpSm;
        delete cylSm;
        delete paraSm;
    }
};

typedef enum {
    XMIN_2,
    XMAX_2,
    YMIN_2,
    YMAX_2,
    DRAW_TICKS_2,
    X_TICK_INT_2,
    Y_TICK_INT_2,
    DRAW_GREY_LINES_2,
    MIN_REC_2,
    MAX_REC_2,
    DEBUG_REGIONS_2
} Setting2ID;

typedef enum {
    XMIN_3,
    XMAX_3,
    YMIN_3,
    YMAX_3,
    ZMIN_3,
    ZMAX_3,
    XSTEP_3,
    YSTEP_3,
    ELEVATION_3,
    ROTATION_3,
    DRAW_CONTOUR_LINES,
    SHADE_SURFACE,
    DRAW_OUTER_BOX
} Setting3RID;

typedef enum { 
    THETA_MIN,
    THETA_MAX,
    THETA_STEP,
    ZSTEP 
} SettingCyl;

typedef enum { 
    TMIN,
    TMAX,
    TSTEP,
    SMIN,
    SMAX,
    SSTEP 
} SettingPara;

}