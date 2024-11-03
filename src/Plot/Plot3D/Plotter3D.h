//
// Created by mmars on 10/6/2024.
//

#pragma once
#include <Menu/MainMenuElements/Settings/MenuItem3DPlotColorSettings.h>
#include <Parsing/AST.h>
#include <Plot/Plot3D/Plot3D.h>
#include <Settings/SettingsManager.h>

namespace bp {
class Plotter3D {
  protected:
    const SettingsManager& sm;

  public:
    explicit Plotter3D(const SettingsManager& sm) : sm{sm} {}
    virtual ~Plotter3D() = default;

    virtual bool Plot(const tiparser::AST* func, Plot3D& plot, BaseColor color) const = 0;
};
}  // namespace bp
