//
// Created by mmars on 8/5/2024.
//

#pragma once
#include <Parsing/Parser.h>
#include <Settings/SettingsManager.h>
#include <Utils/List.h>

#include <cmath>

#include "../Plot3D.h"

namespace bp {

class CylindricalPlotter final : public Plotter3D {
    const SettingsManager& smCyl;

    public:
    CylindricalPlotter(const SettingsManager& sm, const SettingsManager& smCyl)
        : Plotter3D(sm), smCyl{smCyl} {}

    Point3D* CreatePoint(const tiparser::AST* func, double var1, double var2) const override {
        uint8_t vars[2] = {OS_TOK_THETA, OS_TOK_Z};
        double vVals[2] = {var1, var2};
        double r = func->Eval(2, vars, vVals);
        return new Point3D(r * cos(var1), r * sin(var1), var2);
    }

    void ExtractVariableSettings(VariableRange& output1, VariableRange& output2) const override {
        output1.var = OS_TOK_R;
        output1.min = smCyl.GetDouble(THETA_MIN);
        output1.max = smCyl.GetDouble(THETA_MAX);
        output1.step = smCyl.GetDouble(THETA_STEP);
        output2.var = OS_TOK_Z;
        output2.min = sm.GetDouble(ZMIN_3);
        output2.max = sm.GetDouble(ZMAX_3);
        output2.step = smCyl.GetDouble(ZSTEP);
    }
};

}  // namespace bp
