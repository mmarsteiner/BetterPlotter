#pragma once

#include <Settings/SettingsManager.h>
#include <Settings/Settings.h>
#include "Math/Vec4.h"
#include "Renderer.h"

namespace g3d {

class Axes {
    const bp::SettingsManager& s;
    Vec4 pX, mX, pY, mY, pZ, mZ, xLabel, yLabel, zLabel;
public:
    Axes() : s{bp::Settings::GetRect3Settings()}, 
             pX{s.GetDouble(bp::XMAX_3), 0.0, 0.0, 1.0},
             mX{s.GetDouble(bp::XMIN_3), 0.0, 0.0, 1.0},
             pY{0.0, s.GetDouble(bp::YMAX_3), 0.0, 1.0},
             mY{0.0, s.GetDouble(bp::YMIN_3), 0.0, 1.0},
             pZ{0.0, 0.0, s.GetDouble(bp::ZMAX_3), 1.0},
             mZ{0.0, 0.0, s.GetDouble(bp::ZMIN_3), 1.0},
             xLabel{pX.x + (pX.x - mX.x) / 20.0, 0.0, (pZ.z - mZ.z) / 20.0, 1.0},
             yLabel{0.0, pY.y + (pY.y - mY.y) / 20.0, (pZ.z - mZ.z) / 20.0, 1.0},
             zLabel{0.0, 0.0, pZ.z + 2 * (pZ.z - mZ.z) / 20.0, 1.0} {}

    void Render(const Renderer& renderer) const {
        gfx_SetColor(CUSTOM_BLACK);
        renderer.RenderLine(mX, pX);
        renderer.RenderLine(mY, pY);
        renderer.RenderLine(mZ, pZ);
        Vec4 xls, yls, zls;
        renderer.ConvertPoint(xLabel, xls);
        renderer.ConvertPoint(yLabel, yls);
        renderer.ConvertPoint(zLabel, zls);
        gfx_PrintStringXY("x", xls.x, xls.y);
        gfx_PrintStringXY("y", yls.x, yls.y);
        gfx_PrintStringXY("z", zls.x, zls.y);
    }
};

}