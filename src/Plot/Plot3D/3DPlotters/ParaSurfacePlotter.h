#pragma once

#include <Settings/SettingsManager.h>
#include <Utils/List.h>

namespace bp {

class ParaSurfacePlotter final : public Plotter3D {
    const SettingsManager& smPara;
    // TODO: Resolve const stuff to fix this mess
    mutable const tiparser::AST_Triple_Expr* tripleFunc;

    public:
    ParaSurfacePlotter(const SettingsManager& sm, const SettingsManager& smPara) : Plotter3D(sm), smPara{smPara} {}

    bool Plot(const tiparser::AST* func, Plot3D& plot, BaseColor color) const override {
        tripleFunc = dynamic_cast<const tiparser::AST_Triple_Expr*>(func);
        if (tripleFunc == nullptr) {
            dbg_printf("Error: valid comma separated parametric equations were not inputted\n");
            return false;  // still allow the other plots to graph
        }
        return Plotter3D::Plot(func, plot, color);
    }

    Point3D* CreatePoint(const tiparser::AST* /* func */, double var1, double var2) const override {
        uint8_t vars[2] = {OS_TOK_S, OS_TOK_T};
        double vVals[2] = {var1, var2};
        double result[3];
        tripleFunc->TripleEval(2, vars, vVals, result);
        return new Point3D(result[0], result[1], result[2]);
    }

    void ExtractVariableSettings(VariableRange& output1, VariableRange& output2) const override {
        output1.var = OS_TOK_S;
        output1.min = smPara.GetDouble(SMIN);
        output1.max = smPara.GetDouble(SMAX);
        output1.step = smPara.GetDouble(SSTEP);
        output2.var = OS_TOK_T;
        output2.min = sm.GetDouble(TMIN);
        output2.max = sm.GetDouble(TMAX);
        output2.step = smPara.GetDouble(TSTEP);
    }
};

}  // namespace bp
