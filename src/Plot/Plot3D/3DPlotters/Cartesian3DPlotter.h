#pragma once

#include <Plot/Plot3D/Plotter3D.h>
#include <Settings/Settings.h>
#include <ti/tokens.h>

namespace bp {

class Cartesian3DPlotter final : public Plotter3D {
    public:
    Point3D* CreatePoint(const tiparser::AST* func, double var1, double var2) const override {
        uint8_t vars[2] = {OS_TOK_X, OS_TOK_Y};
        double vVals[2] = {var1, var2};
        return new Point3D(var1, var2, func->Eval(2, vars, vVals));
    }

    void ExtractVariableSettings(VariableRange& output1, VariableRange& output2) const override {
        output1.var = OS_TOK_X;
        output1.min = Settings::GetRect3Settings().GetDouble(XMIN_3);
        output1.max = Settings::GetRect3Settings().GetDouble(XMAX_3);
        output1.step = Settings::GetRect3Settings().GetDouble(XSTEP_3);
        output2.var = OS_TOK_Y;
        output2.min = Settings::GetRect3Settings().GetDouble(YMIN_3);
        output2.max = Settings::GetRect3Settings().GetDouble(YMAX_3);
        output2.step = Settings::GetRect3Settings().GetDouble(YSTEP_3);
    }
};

}  // namespace bp
