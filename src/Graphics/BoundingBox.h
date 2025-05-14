#pragma once

#include "Math/Vec4.h"
#include "Renderer.h"
#include "Mesh.h"
#include <Settings/Settings.h>
#include <Settings/SettingsManager.h>

namespace g3d {

class BoundingBox : public Mesh {
public:
    BoundingBox() {
        const bp::SettingsManager& s = bp::Settings::GetRect3Settings();
        AddRow();
        AddPoint(s.GetDouble(bp::XMIN_3), s.GetDouble(bp::YMIN_3), s.GetDouble(bp::ZMIN_3));
        AddPoint(s.GetDouble(bp::XMIN_3), s.GetDouble(bp::YMIN_3), s.GetDouble(bp::ZMAX_3));
        AddRow();
        AddPoint(s.GetDouble(bp::XMIN_3), s.GetDouble(bp::YMAX_3), s.GetDouble(bp::ZMIN_3));
        AddPoint(s.GetDouble(bp::XMIN_3), s.GetDouble(bp::YMAX_3), s.GetDouble(bp::ZMAX_3));
        AddRow();
        AddPoint(s.GetDouble(bp::XMAX_3), s.GetDouble(bp::YMAX_3), s.GetDouble(bp::ZMIN_3));
        AddPoint(s.GetDouble(bp::XMAX_3), s.GetDouble(bp::YMAX_3), s.GetDouble(bp::ZMAX_3));
        AddRow();
        AddPoint(s.GetDouble(bp::XMAX_3), s.GetDouble(bp::YMIN_3), s.GetDouble(bp::ZMIN_3));
        AddPoint(s.GetDouble(bp::XMAX_3), s.GetDouble(bp::YMIN_3), s.GetDouble(bp::ZMAX_3));
        AddRow(); // Last row to get cyclical connecting lines
        AddPoint(s.GetDouble(bp::XMIN_3), s.GetDouble(bp::YMIN_3), s.GetDouble(bp::ZMIN_3));
        AddPoint(s.GetDouble(bp::XMIN_3), s.GetDouble(bp::YMIN_3), s.GetDouble(bp::ZMAX_3));

        for (size_t i = 0; i < GetNumRows(); ++i) {
            for (size_t j = 0; j < GetNumElementsInRow(i); ++j) {
                elements.Get(i)->Get(j)->visible = true;
            }
        }
    }

    void Render(const Renderer& renderer) {
        UpdateScreenLocations(renderer);
        gfx_SetColor(CUSTOM_GREY);
        for (size_t i = 0; i < elements.Size(); ++i) {
            auto& row = *elements.Get(i);
            for (size_t j = 0; j < row.Size(); ++j) {
                const MeshElement& el = *row.Get(j);
                if (ElementExists(i + 1, j)) {
                    const MeshElement& neighbor = *GetElement(i + 1, j);
                    gfx_Line(el.screen.x, el.screen.y, neighbor.screen.x, neighbor.screen.y);
                }
                if (ElementExists(i, j + 1)) {
                    const MeshElement& neighbor = *GetElement(i, j + 1);
                    gfx_Line(el.screen.x, el.screen.y, neighbor.screen.x, neighbor.screen.y);
                }
            }
        }
    }
};

}