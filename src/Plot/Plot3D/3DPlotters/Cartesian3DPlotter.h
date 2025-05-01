#pragma once

#include <Parsing/Parser.h>
#include <Plot/Plot3D/Plot3D.h>
#include <Plot/Plot3D/Plotter3D.h>
#include <Settings/SettingsManager.h>
#include <graphx.h>
#include <ti/tokens.h>

namespace bp {

class Cartesian3DPlotter final : public Plotter3D {
    public:
    explicit Cartesian3DPlotter(const SettingsManager& sm) : Plotter3D(sm) {}

    Point3D* CreatePoint(const tiparser::AST* func, double var1, double var2) const override {
        uint8_t vars[2] = {OS_TOK_X, OS_TOK_Y};
        double vVals[2] = {var1, var2};
        return new Point3D(var1, var2, func->Eval(2, vars, vVals));
    }

    void ExtractVariableSettings(VariableRange& output1, VariableRange& output2) const override {
        output1.var = OS_TOK_X;
        output1.min = sm.GetDouble(XMIN_3);
        output1.max = sm.GetDouble(XMAX_3);
        output1.step = sm.GetDouble(XSTEP_3);
        output2.var = OS_TOK_Y;
        output2.min = sm.GetDouble(YMIN_3);
        output2.max = sm.GetDouble(YMAX_3);
        output2.step = sm.GetDouble(YSTEP_3);
    }
};

}  // namespace bp
