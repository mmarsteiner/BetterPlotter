#pragma once

#include <ti/vars.h>

#include "../MenuItem.h"
#include "Parsing/parser.h"
#include "Plot/Plot3D/3DPlotters/Cartesian3DPlotter.h"
#include "Plot/Plot3D/3DPlotters/CylindricalPlotter.h"
#include "Plot/Plot3D/3DPlotters/ParaSurfacePlotter.h"
#include "Settings/MenuItem3DPlotColorSettings.h"
#include "Settings/SettingsManager.h"

namespace bp {

Plotter3D* CreatePlotter(PlotType3D type, const SettingsManager& cartSm, const SettingsManager& cylSm, const SettingsManager& paraSm);

class MenuItemPlot3D final : public MenuItem {
    private:
    Plot3D plot;
    const SettingsManager& colorSm;
    const SettingsManager& cartSm;
    const SettingsManager& cylSm;
    const SettingsManager& paraSm;

    const char* GetVarName(int varNum) {
        switch (varNum) {
            case 1:
                return OS_VAR_Y1;
            case 2:
                return OS_VAR_Y2;
            case 3:
                return OS_VAR_Y3;
            case 4:
                return OS_VAR_Y4;
            case 5:
                return OS_VAR_Y5;
            case 6:
                return OS_VAR_Y6;
            case 7:
                return OS_VAR_Y7;
        }
        return nullptr;
    }

    public:
    MenuItemPlot3D(const SettingsManager& colorSettings, const SettingsManager& cartSettings, const SettingsManager& cylSettings, const SettingsManager& paraSettings)
        : plot{cartSettings}, colorSm{colorSettings}, cartSm{cartSettings}, cylSm{cylSettings}, paraSm{paraSettings} {
    }
    ~MenuItemPlot3D() {
    }

    bool Run() override {
        bool wasInterrupted = false;
        plot.Open();
        for (int i = 1; i <= 7 && !wasInterrupted; i++) {
            // Collect data about the surface to plot
            PlotType3D type = static_cast<PlotType3D>(colorSm.GetUInt((i - 1) * 2));
            if (type == OFF) {
                continue;
            }
            BaseColor color = static_cast<BaseColor>(colorSm.GetUInt(2 * i - 1));
            tiparser::Parser parser;
            tiparser::AST* toPlot = parser.Parse(GetVarName(i));
            if (toPlot == nullptr) {
                continue;
            }
            Plotter3D* plotter = CreatePlotter(type, cartSm, cylSm, paraSm);
            wasInterrupted = plotter->Plot(toPlot, plot, color);
            delete plotter;
            delete toPlot;
        }
        if (!wasInterrupted) {
            while (os_GetCSC() != sk_Clear) {
            }
        }
        plot.Close();
        return false;
    }

    void GetLabel(char* out) override {
        strcpy(out, "Plot on 3D Axes");
    }

    void AppendLabel(char* out) override {
        strcat(out, "Plot on 3D Axes");
    }
};

Plotter3D* CreatePlotter(PlotType3D type, const SettingsManager& cartSm, const SettingsManager& cylSm, const SettingsManager& paraSm) {
    if (type == CARTESIAN) {
        return new Cartesian3DPlotter(cartSm);
    } else if (type == CYLINDRICAL) {
        return new CylindricalPlotter(cartSm, cylSm);
    } else if (type == PARA_SURF) {
        return new ParaSurfacePlotter(cartSm, paraSm);
    } else {
        return nullptr;
    }
}

}  // namespace bp