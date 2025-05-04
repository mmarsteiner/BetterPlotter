//
// Created by mmars on 7/30/2024.
//

#pragma once

#include <Menu/MenuItem.h>
#include <Plot/Plot2D/Plot2D.h>
#include <Plot/Plot2D/QuadtreePlotter.h>

namespace bp {

class MenuItem2DImplicit final : public MenuItem {

    public:
    bool Run() override {
        Plot2D plot;
        plot.Open();
        tiparser::Parser parser;
        tiparser::AST* func = parser.Parse(OS_VAR_Y1);
        QuadtreePlotter plotter;
        bool interrupted = plotter.plotImplicit(func, plot);
        if (!interrupted) {
            while (os_GetCSC() != sk_Clear) {
            }
        }
        plot.Close();
        return false;
    }

    void GetLabel(char* out) override { strcpy(out, "Plot 2D y1(x,y)=0"); }
    void AppendLabel(char* out) override { strcat(out, "Plot 2D y1(x,y)=0"); }
};

}  // namespace bp
