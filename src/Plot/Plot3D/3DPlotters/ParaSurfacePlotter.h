#pragma once

#include <Settings/SettingsManager.h>
#include <Utils/List.h>
#include <Parsing/Parser.h>
#include <Plot/Plot3D/Plotter3D.h>

namespace bp {

class ParaSurfacePlotter final : public Plotter3D {
    // TODO: Resolve const stuff to fix this mess
    mutable const tiparser::AST_Triple_Expr* tripleFunc;

    public:
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
        output1.min = Settings::GetParaSettings().GetDouble(SMIN);
        output1.max = Settings::GetParaSettings().GetDouble(SMAX);
        output1.step = Settings::GetParaSettings().GetDouble(SSTEP);
        output2.var = OS_TOK_T;
        output2.min = Settings::GetRect3Settings().GetDouble(TMIN);
        output2.max = Settings::GetRect3Settings().GetDouble(TMAX);
        output2.step = Settings::GetParaSettings().GetDouble(TSTEP);
    }
};

}  // namespace bp
